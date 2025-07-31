#pragma once
#include "../../Core/framework.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <atomic>
#include <future>

namespace SparkEngine {
    class RenderCommand {
    public:
        virtual ~RenderCommand() = default;
        virtual void Execute() = 0;
        virtual const char* GetName() const { return "RenderCommand"; }
    };

    template<typename Func>
    class LambdaRenderCommand : public RenderCommand {
    private:
        Func m_function;
        std::string m_name;

    public:
        explicit LambdaRenderCommand(Func&& func, const std::string& name = "Lambda") 
            : m_function(std::forward<Func>(func)), m_name(name) {}

        void Execute() override {
            m_function();
        }

        const char* GetName() const override { return m_name.c_str(); }
    };

    class RenderCommandQueue {
    private:
        std::queue<std::unique_ptr<RenderCommand>> m_commands;
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_running{true};
        std::atomic<size_t> m_commandCount{0};

    public:
        RenderCommandQueue() = default;
        ~RenderCommandQueue() { Shutdown(); }

        template<typename Func>
        void Submit(Func&& func, const std::string& name = "Command") {
            auto command = std::make_unique<LambdaRenderCommand<Func>>(
                std::forward<Func>(func), name);

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_commands.push(std::move(command));
                m_commandCount++;
            }
            m_condition.notify_one();
        }

        template<typename Func>
        auto SubmitAndWait(Func&& func, const std::string& name = "SyncCommand") -> decltype(func()) {
            using ReturnType = decltype(func());

            if constexpr (std::is_void_v<ReturnType>) {
                std::promise<void> promise;
                auto future = promise.get_future();

                Submit([func = std::forward<Func>(func), &promise]() mutable {
                    func();
                    promise.set_value();
                }, name);

                future.wait();
            } else {
                std::promise<ReturnType> promise;
                auto future = promise.get_future();

                Submit([func = std::forward<Func>(func), &promise]() mutable {
                    promise.set_value(func());
                }, name);

                return future.get();
            }
        }

        void ExecuteCommands() {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (!m_commands.empty()) {
                auto command = std::move(m_commands.front());
                m_commands.pop();
                m_commandCount--;

                lock.unlock();
                command->Execute();
                lock.lock();
            }
        }

        void WaitAndExecute() {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this] { return !m_commands.empty() || !m_running; });

            if (m_running) {
                ExecuteCommands();
            }
        }

        void Flush() {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this] { return m_commands.empty(); });
        }

        void Shutdown() {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_running = false;
            }
            m_condition.notify_all();
        }

        size_t GetCommandCount() const { return m_commandCount; }
        bool IsEmpty() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_commands.empty();
        }
    };

    // Thread-safe render context
    class RenderThreadManager {
    private:
        std::unique_ptr<std::thread> m_renderThread;
        RenderCommandQueue m_commandQueue;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_deferredContext;
        ComPtr<ID3D11CommandList> m_commandList;
        std::atomic<bool> m_running{false};

    public:
        bool Initialize(ID3D11Device* device);
        void Shutdown();

        template<typename Func>
        void SubmitCommand(Func&& func, const std::string& name = "Command") {
            m_commandQueue.Submit(std::forward<Func>(func), name);
        }

        template<typename Func>
        auto SubmitAndWait(Func&& func, const std::string& name = "SyncCommand") {
            return m_commandQueue.SubmitAndWait(std::forward<Func>(func), name);
        }

        void FlushCommands() { m_commandQueue.Flush(); }

        ID3D11DeviceContext* GetDeferredContext() { return m_deferredContext.Get(); }

    private:
        void RenderThreadFunction();
    };
}