#pragma once
#include"Engine/GameObject.h"
class BulletBase : public GameObject {
protected:
    XMFLOAT3 moveDirection;
    float speed;
public:
    void Move() {
        transform_.position_.x += moveDirection.x;
        transform_.position_.z += moveDirection.z;
    }

    void CheckOutOfBounds(float range = 10.0f) {
        if (transform_.position_.x < -range || transform_.position_.x > range ||
            transform_.position_.z < -range || transform_.position_.z > range) {
            KillMe();
        }
    }
};
