
// inline mat4 lookAtv2(vec3 camPosition, vec3 lookAtPoint, vec3 upVector)
// {
//     mat4 returnMatrix = {};

//     vec3 zaxis = Normalize(camPosition - lookAtPoint);
//     vec3 xaxis = Normalize(Cross(upVector, zaxis));
//     vec3 yaxis = Cross(zaxis, xaxis);

//     mat4 translation = Translation4(camPosition);
//     mat4 result = {};
//     result.columns[0] = V4(xaxis.x, yaxis.x, zaxis.x, 0.0f);
//     result.columns[1] = V4(xaxis.y, yaxis.y, zaxis.y, 0.0f);
//     result.columns[2] = V4(xaxis.z, yaxis.z, zaxis.z, 0.0f);
//     result.columns[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
    
//     returnMatrix = result * translation;

//     return returnMatrix;
// }
void InGameCameraInit()
{

    Camera *cam = &Zayn->camera;
    cam->currentSpeed = 0;
    cam->targetSpeed = 20.0f;
    cam->targetTurnSpeed = 160.0f;

    cam->pos = glm::vec3(0, 0, 0.5f);
    cam->front = glm::vec3(-1, 0, 0);
    cam->up = glm::vec3(0, 0, 1);
}

void FirstPersonCameraController()
{
    //

    // Camera *cam = &Zayn->camera;

    // if (InputHeld(Keyboard, Input_W))
    // {
    //     cam->currentSpeed = cam->targetSpeed;
    //     /// player->isWalkingForwardOrBackward = true;
    // }
    // else if (InputHeld(Keyboard, Input_S))
    // {
    //     cam->currentSpeed = -cam->targetSpeed;
    //     /// player->isWalkingForwardOrBackward = true;
    // }
    // else
    // {
    //     cam->currentSpeed = 0;
    //     /// player->isWalkingForwardOrBackward = false;
    // }

    // if (InputHeld(Keyboard, Input_A))
    // {
    //     cam->currentTurnSpeed = cam->targetTurnSpeed;
    // }
    // else if (InputHeld(Keyboard, Input_D))
    // {
    //     cam->currentTurnSpeed = -cam->targetTurnSpeed;
    // }
    // else
    // {
    //     cam->currentTurnSpeed = 0;
    // }

    // cam->targetRotY += DegToRad(cam->currentTurnSpeed /* Game->deltaTime*/);
    // cam->targetPos.x += cam->currentSpeed * /*Game->deltaTime **/ (sinf(cam->targetRotY));
    // cam->targetPos.z += cam->currentSpeed * /*Game->deltaTime **/ (cosf(cam->targetRotY));


    // cam->yaw = -90 - RadToDeg(cam->targetRotY);
    // vec3 camDelta = cam->pos;
    // cam->pos = -cam->targetPos - (Normalize(Cross(cam->front, cam->up)) * cam->speed * 0.01f) * sinf(cam->walkingModTime * 6) * 0.6f; // horiz walk sim
    // cam->pos =  -cam->targetPos - (cam->up * cam->speed * 0.01f) * sinf(cam->walkingModTime * 12) * 1.0f;  // vert walk sim
    // cam->pos.y -= 5;
    // // cam->resetWalk = false;
    
    // cam->pitch = 0;
     
    // // if (cam->isWalkingForwardOrBackward)
    // // {
    // //    // cam->walkingModTime += Game->deltaTime;
    // // }

    // cam->front.x = cosf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    // cam->front.y = sinf(DegToRad(cam->pitch));
    // cam->front.z = sinf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    // cam->front = Normalize(cam->front);

    // cam->view = lookAtv2(cam->pos, cam->pos + cam->front, V3(0, 1, 0));


    //
}

void UpdateCameraInGame()
{
    Camera *cam = &Zayn->camera;

    FirstPersonCameraController();
}