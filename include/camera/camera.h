#ifndef _LIGHTMAN_CAMERA_H
#define _LIGHTMAN_CAMERA_H

#include "utils/ref.h"
#include "math/matrix4x4.h"
#include "math/vector.h"

using namespace lightman::utils;
using namespace lightman::math;

namespace lightman
{
    class Camera : public Ref
    {
    public:
        enum class CameraType : uint8_t
        {
            PERSPECTIVE,
            ORTHO
        };
        enum class FovDirection : uint8_t
        {
            HORIZONTAL,
            VERTICAL
        };
        virtual CameraType GetCameraType() = 0 ;
        void LookAt(float eye[3], float target[3], float up[3]);
        virtual ~Camera();
        void setProjection(float fov, float near, float far, float aspect,
                                  Camera::FovDirection direction = Camera::FovDirection::VERTICAL);
        void setProjection(float left, float right, float bottom, float up, float near, float far);
        Matrix4X4 GetViewMatrix();
        Matrix4X4 GetProjectionMatrix();
        Matrix4X4 GetProjectionViewMatrix();
        void SetCameraMovingConsumed(){isCameraMovingConsumed = true;};
        bool IsCameraMovingConsumed(){return isCameraMovingConsumed;};
        Vector3 GetWorldPosition();
    protected:
        Camera();
        float m_near{};
        float m_far{};

        float m_eye[3];
        float m_target[3];
        float m_up[3];
        Matrix4X4 m_worldToCamera;
        Matrix4X4 m_cameraToWorld;
        Matrix4X4 m_cameraToScreen;
        Matrix4X4 m_worldToSreen;
        bool isCameraMovingConsumed = false;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        CameraType GetCameraType() override {return Camera::CameraType::PERSPECTIVE;};
        PerspectiveCamera(/* args */) = default;
        ~PerspectiveCamera() = default;        
    };

    class OrthogrpicCamera : public Camera
    {
    public:
        CameraType GetCameraType() override {return Camera::CameraType::ORTHO;};
        OrthogrpicCamera(/* args */);
        ~OrthogrpicCamera() = default;
    };    
}
#endif // _LIGHTMAN_CAMERA_H
