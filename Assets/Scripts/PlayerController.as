// PlayerController.as - Example AngelScript component
class PlayerController {
    uint entityId;  // Set automatically by engine
    
    float moveSpeed = 5.0f;
    float jumpForce = 10.0f;
    bool isGrounded = true;
    
    void Start() {
        Print("PlayerController started for entity: " + entityId);
    }
    
    void Update(float deltaTime) {
        HandleMovement(deltaTime);
        HandleJump();
    }
    
    void HandleMovement(float deltaTime) {
        Vector3 movement = Vector3(0, 0, 0);
        
        if (GetKey("W")) movement.z += 1.0f;
        if (GetKey("S")) movement.z -= 1.0f;
        if (GetKey("A")) movement.x -= 1.0f;
        if (GetKey("D")) movement.x += 1.0f;
        
        if (movement.Length() > 0) {
            movement = movement.Normalized() * moveSpeed * deltaTime;
            
            Transform@ transform = GetTransform(entityId);
            if (transform !is null) {
                transform.position = transform.position + movement;
            }
        }
    }
    
    void HandleJump() {
        if (GetKeyDown(" ") && isGrounded) {
            Print("Player jumped!");
            isGrounded = false;
        }
    }
    
    void OnCollision(uint otherEntityId) {
        Print("Player collided with entity: " + otherEntityId);
        isGrounded = true;
    }
}
