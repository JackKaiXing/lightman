#ifndef _LIGHTMAN_CAMERA_H
#define _LIGHTMAN_CAMERA_H

#include "utils/ref.h"
#include "math/matrix4x4.h"

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
        virtual CameraType GetCameraType() = 0 ;
        void LookAt(float eye[3], float target[3], float up[3]);
        virtual ~Camera();
    protected:
        Camera();
    private:
        float m_near{};
        float m_far{};

        float m_eye[3];
        float m_target[3];
        float m_up[3];
        Matrix4X4 m_worldToCamera;
        Matrix4X4 m_cameraToWorld;
    };

    class PerspectiveCamera : public Camera
    {
    public:
        CameraType GetCameraType() override {return Camera::CameraType::PERSPECTIVE;};
        PerspectiveCamera(/* args */) = default;
        ~PerspectiveCamera() = default;
    private:
        Matrix4X4 m_cameraToScreen;
    };

    class OrthogrpicCamera : public Camera
    {
    public:
        CameraType GetCameraType() override {return Camera::CameraType::ORTHO;};
        OrthogrpicCamera(/* args */);
        ~OrthogrpicCamera();
    private:
        Matrix4X4 m_cameraToScreen;
    };    
}
#endif // _LIGHTMAN_CAMERA_H