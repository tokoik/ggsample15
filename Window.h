#pragma once

/*
** �Q�[���O���t�B�b�N�X���_�p�⏕�v���O���� GLFW3 ��
**

Copyright (c) 2011-2017 Kohe Tokoi. All Rights Reserved.

Permission is hereby granted, free of charge,  to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction,  including without limitation the rights
to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell
copies or substantial portions of the Software.

The above  copyright notice  and this permission notice  shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY,
FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL
KOHE TOKOI  BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY,  WHETHER IN
AN ACTION  OF CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM,  OUT OF  OR  IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**
*/

// �⏕�v���O����
#include "gg.h"
using namespace gg;

// Oculus Rift SDK ���C�u���� (LibOVR) �̑g�ݍ���
#if defined(USE_OCULUS_RIFT)
#  if defined(_WIN32)
#    define GLFW_EXPOSE_NATIVE_WIN32
#    define GLFW_EXPOSE_NATIVE_WGL
#    include "glfw3native.h"
#    define OVR_OS_WIN32
#    undef APIENTRY
#    pragma comment(lib, "LibOVR.lib")
#  endif
#  include <OVR_CAPI_GL.h>
#  include <Extras/OVR_Math.h>
#  if OVR_PRODUCT_VERSION > 0
#    include <dxgi.h> // GetDefaultAdapterLuid �̂���
#    pragma comment(lib, "dxgi.lib")
inline ovrGraphicsLuid GetDefaultAdapterLuid()
{
  ovrGraphicsLuid luid = ovrGraphicsLuid();

#    if defined(_WIN32)
  IDXGIFactory *factory(nullptr);

  if (SUCCEEDED(CreateDXGIFactory(IID_PPV_ARGS(&factory))))
  {
    IDXGIAdapter *adapter(nullptr);

    if (SUCCEEDED(factory->EnumAdapters(0, &adapter)))
    {
      DXGI_ADAPTER_DESC desc;

      adapter->GetDesc(&desc);
      memcpy(&luid, &desc.AdapterLuid, sizeof luid);
      adapter->Release();
    }

    factory->Release();
  }
#    endif

  return luid;
}

inline int Compare(const ovrGraphicsLuid& lhs, const ovrGraphicsLuid& rhs)
{
  return memcmp(&lhs, &rhs, sizeof(ovrGraphicsLuid));
}
#  endif
#endif

// �W�����C�u����
#include <cstdlib>
#include <iostream>

//
// �E�B���h�E�֘A�̏���
//
class Window
{
  // �E�B���h�E�̎��ʎq
  GLFWwindow *window;

  // �r���[�|�[�g�̕��ƍ���
  GLsizei width, height;

  // �r���[�|�[�g�̃A�X�y�N�g��
  GLfloat aspect;

  // �V�t�g�L�[
  bool shift_key;

  // �R���g���[���L�[
  bool control_key;

  // ���L�[
  int arrow[2];

  // �V�t�g�L�[�������Ȃ�����L�[
  int shift_arrow[2];

  // �R���g���[���L�[�������Ȃ�����L�[
  int control_arrow[2];

  // �}�E�X�̌��݈ʒu
  double mouse_x, mouse_y;

  // �}�E�X�z�C�[���̉�]��
  double wheel_rotation;

  // �V�t�g�������Ȃ���}�E�X�z�C�[���̉�]��
  double shift_wheel_rotation;

  // �R���g���[���������Ȃ���}�E�X�z�C�[���̉�]��
  double control_wheel_rotation;

  // ���h���b�O�ɂ��g���b�N�{�[��
  GgTrackball trackball_left;

  // �E�h���b�O�ɂ��g���b�N�{�[��
  GgTrackball trackball_right;

#if defined(USE_OCULUS_RIFT)
  //
  // Oculus Rift
  //

  // Oculus Rift �̃Z�b�V����
  ovrSession session;

  // Oculus Rift �̏��
  ovrHmdDesc hmdDesc;

  // Oculus Rift �̃X�N���[���̃T�C�Y
  GLfloat screen[ovrEye_Count][4];

  // Oculus Rift �̃X�N���[���̃w�b�h�g���b�L���O�ʒu����̕ψ�
  GLfloat offset[ovrEye_Count][3];

  // Oculus Rift �\���p�� FBO
  GLuint oculusFbo[ovrEye_Count];

  // �~���[�\���p�� FBO
  GLuint mirrorFbo;

#  if OVR_PRODUCT_VERSION > 0
  // Oculus Rift �ɑ���`��f�[�^
  ovrLayerEyeFov layerData;

  // Oculus Rift �Ƀ����_�����O����t���[���̔ԍ�
  long long frameIndex;

  // Oculus Rift �\���p�� FBO �̃f�v�X�e�N�X�`��
  GLuint oculusDepth[ovrEye_Count];

  // �~���[�\���p�� FBO �̃T�C�Y
  int mirrorWidth, mirrorHeight;

  // �~���[�\���p�� FBO �̃J���[�e�N�X�`��
  ovrMirrorTexture mirrorTexture;

#  else

  // Oculus Rift �ɑ���`��f�[�^
  ovrLayer_Union layerData;

  // Oculus Rift �̃����_�����O���
  ovrEyeRenderDesc eyeRenderDesc[ovrEye_Count];

  // Oculus Rift �̎��_���
  ovrPosef eyePose[ovrEye_Count];

  // �~���[�\���p�� FBO �̃J���[�e�N�X�`��
  ovrGLTexture *mirrorTexture;
#  endif
#endif

  // �R�s�[�R���X�g���N�^�𕕂���
  Window(const Window &w);

  // ����𕕂���
  Window &operator=(const Window &w);

public:

  //
  // �R���X�g���N�^
  //
  Window(const char *title = "GLFW Window", int width = 640, int height = 480,
    int fullscreen = 0, GLFWwindow *share = nullptr)
    : window(nullptr)
  {
    // �������ςȂ� true
    static bool initialized(false);

    // GLFW ������������Ă��Ȃ����
    if (!initialized)
    {
      // GLFW ������������
      if (glfwInit() == GL_FALSE) return;

      // �v���O�����I�����̏�����o�^
      atexit(glfwTerminate);

#if defined(USE_OCULUS_RIFT)
      // Oculus Rift (LibOVR) ������������
      if (OVR_FAILURE(ovr_Initialize(nullptr))) return;

      // �v���O�����I�����ɂ� LibOVR ���I������
      atexit(ovr_Shutdown);

      // LUID �� OpenGL �ł͎g���Ă��Ȃ��炵��
      ovrGraphicsLuid luid;

      // Oculus Rift �̃Z�b�V�������쐬����
      session = nullptr;
      if (OVR_FAILURE(ovr_Create(&session, &luid))) return;

      // Oculus Rift �ւ̃����_�����O�Ɏg�� FBO �̏����l��ݒ肷��
      for (int eye = 0; eye < ovrEye_Count; ++eye) oculusFbo[eye] = 0;

      // �~���[�\���Ɏg�� FBO �̏����l��ݒ肷��
      mirrorFbo = 0;
      mirrorTexture = nullptr;

#  if OVR_PRODUCT_VERSION > 0
      // �f�t�H���g�̃O���t�B�b�N�X�A�_�v�^���g���Ă��邩�m���߂�
      if (Compare(luid, GetDefaultAdapterLuid())) return;

      // Asynchronous TimeWarp �����Ɏg���t���[���ԍ��̏����l��ݒ肷��
      frameIndex = 0LL;

      // Oculus Rift �ւ̃����_�����O�Ɏg�� FBO �̃f�v�X�e�N�X�`���̏����l��ݒ肷��
      for (int eye = 0; eye < ovrEye_Count; ++eye) oculusDepth[eye] = 0;
#  endif

      // Oculus Rift �ł̓_�u���o�b�t�@�����O���Ȃ�
      glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

      // Oculus Rift �ł� SRGB �Ń����_�����O����
      glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
#endif

      // OpenGL Version 3.2 Core Profile ��I������
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      // ���L�[�ƃ}�E�X����̏����l��ݒ肷��
      arrow[0] = arrow[1] = 0;
      shift_arrow[0] = shift_arrow[1] = 0;
      control_arrow[0] = control_arrow[1] = 0;
      wheel_rotation = shift_wheel_rotation = control_wheel_rotation = 0.0;

      // �������ς݂̈������
      initialized = true;
    }

    // �f�B�X�v���C�̏��
    GLFWmonitor *monitor(nullptr);

    // �t���X�N���[���\��
    if (fullscreen > 0)
    {
      // �ڑ�����Ă��郂�j�^�̐��𐔂���
      int mcount;
      GLFWmonitor **const monitors = glfwGetMonitors(&mcount);

      // �Z�J���_�����j�^������΂�����g��
      if (fullscreen > mcount) fullscreen = mcount;
      monitor = monitors[fullscreen - 1];

      // ���j�^�̃��[�h�𒲂ׂ�
      const GLFWvidmode *mode(glfwGetVideoMode(monitor));

      // �E�B���h�E�̃T�C�Y���f�B�X�v���C�̃T�C�Y�ɂ���
      width = mode->width;
      height = mode->height;
    }

    // GLFW �̃E�B���h�E���쐬����
    window = glfwCreateWindow(width, height, title, monitor, share);

    // �E�B���h�E���쐬�ł��Ȃ���Ζ߂�
    if (!window) return;

    // ���݂̃E�B���h�E�������Ώۂɂ���
    glfwMakeContextCurrent(window);

    // �Q�[���O���t�B�b�N�X���_�̓s���ɂ�鏉�������s��
    ggInit();

    // ���̃C���X�^���X�� this �|�C���^���L�^���Ă���
    glfwSetWindowUserPointer(window, this);

    // �L�[�{�[�h�𑀍삵�����̏�����o�^����
    glfwSetKeyCallback(window, keyboard);

    // �}�E�X�{�^���𑀍삵���Ƃ��̏�����o�^����
    glfwSetMouseButtonCallback(window, mouse);

    // �}�E�X�z�C�[�����쎞�ɌĂяo��������o�^����
    glfwSetScrollCallback(window, wheel);

    // �E�B���h�E�̃T�C�Y�ύX���ɌĂяo��������o�^����
    glfwSetFramebufferSizeCallback(window, resize);

#if defined(USE_OCULUS_RIFT)
    // Oculus Rift �̏������o��
    hmdDesc = ovr_GetHmdDesc(session);

#  if defined(_DEBUG)
    // Oculus Rift �̏���\������
    std::cout
      << "\nProduct name: " << hmdDesc.ProductName
      << "\nResolution:   " << hmdDesc.Resolution.w << " x " << hmdDesc.Resolution.h
      << "\nDefault Fov:  (" << hmdDesc.DefaultEyeFov[ovrEye_Left].LeftTan
      << "," << hmdDesc.DefaultEyeFov[ovrEye_Left].DownTan
      << ") - (" << hmdDesc.DefaultEyeFov[ovrEye_Left].RightTan
      << "," << hmdDesc.DefaultEyeFov[ovrEye_Left].UpTan
      << ")\n              (" << hmdDesc.DefaultEyeFov[ovrEye_Right].LeftTan
      << "," << hmdDesc.DefaultEyeFov[ovrEye_Right].DownTan
      << ") - (" << hmdDesc.DefaultEyeFov[ovrEye_Right].RightTan
      << "," << hmdDesc.DefaultEyeFov[ovrEye_Right].UpTan
      << ")\nMaximum Fov:  (" << hmdDesc.MaxEyeFov[ovrEye_Left].LeftTan
      << "," << hmdDesc.MaxEyeFov[ovrEye_Left].DownTan
      << ") - (" << hmdDesc.MaxEyeFov[ovrEye_Left].RightTan
      << "," << hmdDesc.MaxEyeFov[ovrEye_Left].UpTan
      << ")\n              (" << hmdDesc.MaxEyeFov[ovrEye_Right].LeftTan
      << "," << hmdDesc.MaxEyeFov[ovrEye_Right].DownTan
      << ") - (" << hmdDesc.MaxEyeFov[ovrEye_Right].RightTan
      << "," << hmdDesc.MaxEyeFov[ovrEye_Right].UpTan
      << ")\n" << std::endl;
#  endif

    // Oculus Rift �ɓ]������`��f�[�^���쐬����
#  if OVR_PRODUCT_VERSION > 0
    layerData.Header.Type = ovrLayerType_EyeFov;
#  else
    layerData.Header.Type = ovrLayerType_EyeFovDepth;
#  endif
    layerData.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // OpenGL �Ȃ̂ō��������_

    // Oculus Rift �\���p�� FBO ���쐬����
    for (int eye = 0; eye < ovrEye_Count; ++eye)
    {
      // Oculus Rift �̎�����擾����
      const auto &fov(hmdDesc.DefaultEyeFov[ovrEyeType(eye)]);

      // Oculus Rift �\���p�� FBO �̃T�C�Y�����߂�
      const auto textureSize(ovr_GetFovTextureSize(session, ovrEyeType(eye), fov, 1.0f));

      // Oculus Rift �\���p�� FBO �̃A�X�y�N�g������߂�
      aspect = static_cast<GLfloat>(textureSize.w) / static_cast<GLfloat>(textureSize.h);

      // Oculus Rift �̃X�N���[���̃T�C�Y��ۑ�����
      screen[eye][0] = -fov.LeftTan;
      screen[eye][1] = fov.RightTan;
      screen[eye][2] = -fov.DownTan;
      screen[eye][3] = fov.UpTan;

#  if OVR_PRODUCT_VERSION > 0
      // �`��f�[�^�Ɏ����ݒ肷��
      layerData.Fov[eye] = fov;

      // �`��f�[�^�Ƀr���[�|�[�g��ݒ肷��
      layerData.Viewport[eye].Pos = OVR::Vector2i(0, 0);
      layerData.Viewport[eye].Size = textureSize;

      // Oculus Rift �\���p�� FBO �̃J���[�o�b�t�@�Ƃ��Ďg���e�N�X�`���Z�b�g�̓���
      const ovrTextureSwapChainDesc colorDesc =
      {
        ovrTexture_2D,                    // Type
        OVR_FORMAT_R8G8B8A8_UNORM_SRGB,   // Format
        1,                                // ArraySize
        textureSize.w,                    // Width
        textureSize.h,                    // Height
        1,                                // MipLevels
        1,                                // SampleCount
        ovrFalse,                         // StaticImage
        0, 0
      };

      // Oculus Rift �\���p�� FBO �̃����_�[�^�[�Q�b�g�Ƃ��Ďg���e�N�X�`���`�F�C�����쐬����
      layerData.ColorTexture[eye] = nullptr;
      if (OVR_SUCCESS(ovr_CreateTextureSwapChainGL(session, &colorDesc, &layerData.ColorTexture[eye])))
      {
        // �쐬�����e�N�X�`���`�F�C���̒������擾����
        int length(0);
        if (OVR_SUCCESS(ovr_GetTextureSwapChainLength(session, layerData.ColorTexture[eye], &length)))
        {
          // �e�N�X�`���`�F�C���̌X�̗v�f�ɂ���
          for (int i = 0; i < length; ++i)
          {
            // �e�N�X�`���̃p�����[�^��ݒ肷��
            GLuint texId;
            ovr_GetTextureSwapChainBufferGL(session, layerData.ColorTexture[eye], i, &texId);
            glBindTexture(GL_TEXTURE_2D, texId);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          }
        }

        // Oculus Rift �\���p�� FBO �̃f�v�X�o�b�t�@�Ƃ��Ďg���e�N�X�`�����쐬����
        glGenTextures(1, &oculusDepth[eye]);
        glBindTexture(GL_TEXTURE_2D, oculusDepth[eye]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, textureSize.w, textureSize.h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      }

#  else

      // �`��f�[�^�Ɏ����ݒ肷��
      layerData.EyeFov.Fov[eye] = fov;

      // �`��f�[�^�Ƀr���[�|�[�g��ݒ肷��
      layerData.EyeFov.Viewport[eye].Pos = OVR::Vector2i(0, 0);
      layerData.EyeFov.Viewport[eye].Size = textureSize;

      // Oculus Rift �\���p�� FBO �̃J���[�o�b�t�@�Ƃ��Ďg���e�N�X�`���Z�b�g���쐬����
      ovrSwapTextureSet *colorTexture;
      ovr_CreateSwapTextureSetGL(session, GL_SRGB8_ALPHA8, textureSize.w, textureSize.h, &colorTexture);
      layerData.EyeFov.ColorTexture[eye] = colorTexture;

      // Oculus Rift �\���p�� FBO �̃f�v�X�o�b�t�@�Ƃ��Ďg���e�N�X�`���Z�b�g���쐬����
      ovrSwapTextureSet *depthTexture;
      ovr_CreateSwapTextureSetGL(session, GL_DEPTH_COMPONENT32F, textureSize.w, textureSize.h, &depthTexture);
      layerData.EyeFovDepth.DepthTexture[eye] = depthTexture;

      // Oculus Rift �̃����Y�␳���̐ݒ�l���擾����
      eyeRenderDesc[eye] = ovr_GetRenderDesc(session, ovrEyeType(eye), fov);

      // Oculus Rift �̃X�N���[���̃w�b�h�g���b�L���O�ʒu����̕ψʂ�ۑ�����
      offset[eye][0] = eyeRenderDesc[eye].HmdToEyeViewOffset.x;
      offset[eye][1] = eyeRenderDesc[eye].HmdToEyeViewOffset.y;
      offset[eye][2] = eyeRenderDesc[eye].HmdToEyeViewOffset.z;
#  endif
    }

#  if OVR_PRODUCT_VERSION > 0
    // �p���̃g���b�L���O�ɂ����鏰�̍����� 0 �ɐݒ肷��
    ovr_SetTrackingOriginType(session, ovrTrackingOrigin_FloorLevel);

    // �~���[�\���p�� FBO ���쐬����
    const ovrMirrorTextureDesc mirrorDesc =
    {
      OVR_FORMAT_R8G8B8A8_UNORM_SRGB,   // Format
      mirrorWidth = width,              // Width
      mirrorHeight = height,            // Height
      0                                 // Flags
    };

    // �~���[�\���p�� FBO �̃J���[�o�b�t�@�Ƃ��Ďg���e�N�X�`�����쐬����
    if (OVR_SUCCESS(ovr_CreateMirrorTextureGL(session, &mirrorDesc, &mirrorTexture)))
    {
      // �쐬�����e�N�X�`���̃e�N�X�`�����𓾂�
      GLuint texId;
      if (OVR_SUCCESS(ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId)))
      {
        // �쐬�����e�N�X�`�����~���[�\���p�� FBO �ɃJ���[�o�b�t�@�Ƃ��đg�ݍ���
        glGenFramebuffers(1, &mirrorFbo);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFbo);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
        glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      }
    }

#  else

    // �~���[�\���p�� FBO ���쐬����
    if (OVR_SUCCESS(ovr_CreateMirrorTextureGL(session, GL_SRGB8_ALPHA8, width, height, reinterpret_cast<ovrTexture **>(&mirrorTexture))))
    {
      glGenFramebuffers(1, &mirrorFbo);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFbo);
      glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
      glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    }
#  endif

    // Oculus Rift �̃����_�����O�p�� FBO ���쐬����
    glGenFramebuffers(ovrEye_Count, oculusFbo);

    // Oculus Rift �Ƀ����_�����O����Ƃ��� sRGB �J���[�X�y�[�X���g��
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Oculus Rift �ւ̕\���ł̓X���b�v�Ԋu��҂��Ȃ�
    glfwSwapInterval(0);

#else

    // �X���b�v�Ԋu��҂�
    glfwSwapInterval(1);
#endif

    // �r���[�|�[�g�Ɠ��e�ϊ��s�������������
    resize(window, width, height);
  }

  //
  // �f�X�g���N�^
  //
  virtual ~Window()
  {
    // �E�B���h�E���쐬����Ă��Ȃ���Ζ߂�
    if (!window) return;

#if defined(USE_OCULUS_RIFT)
    // �~���[�\���p�� FBO ���폜����
    if (mirrorFbo) glDeleteFramebuffers(1, &mirrorFbo);

    // �~���[�\���Ɏg�����e�N�X�`�����J������
    if (mirrorTexture)
    {
#  if OVR_PRODUCT_VERSION > 0
      ovr_DestroyMirrorTexture(session, mirrorTexture);
#  else
      glDeleteTextures(1, &mirrorTexture->OGL.TexId);
      ovr_DestroyMirrorTexture(session, reinterpret_cast<ovrTexture *>(mirrorTexture));
#  endif
    }

    // Oculus Rift �̃����_�����O�p�� FBO ���폜����
    glDeleteFramebuffers(ovrEye_Count, oculusFbo);

    // Oculus Rift �\���p�� FBO ���폜����
    for (int eye = 0; eye < ovrEye_Count; ++eye)
    {
#  if OVR_PRODUCT_VERSION > 0
      // �����_�����O�^�[�Q�b�g�Ɏg�����e�N�X�`�����J������
      if (layerData.ColorTexture[eye])
      {
        ovr_DestroyTextureSwapChain(session, layerData.ColorTexture[eye]);
        layerData.ColorTexture[eye] = nullptr;
      }

      // �f�v�X�o�b�t�@�Ƃ��Ďg�����e�N�X�`�����J������
      glDeleteTextures(1, &oculusDepth[eye]);
      oculusDepth[eye] = 0;

#  else

      // �����_�����O�^�[�Q�b�g�Ɏg�����e�N�X�`�����J������
      auto *const colorTexture(layerData.EyeFov.ColorTexture[eye]);
      for (int i = 0; i < colorTexture->TextureCount; ++i)
      {
        const auto *const ctex(reinterpret_cast<ovrGLTexture *>(&colorTexture->Textures[i]));
        glDeleteTextures(1, &ctex->OGL.TexId);
      }
      ovr_DestroySwapTextureSet(session, colorTexture);

      // �f�v�X�o�b�t�@�Ƃ��Ďg�����e�N�X�`�����J������
      auto *const depthTexture(layerData.EyeFovDepth.DepthTexture[eye]);
      for (int i = 0; i < depthTexture->TextureCount; ++i)
      {
        const auto *const dtex(reinterpret_cast<ovrGLTexture *>(&depthTexture->Textures[i]));
        glDeleteTextures(1, &dtex->OGL.TexId);
      }
      ovr_DestroySwapTextureSet(session, depthTexture);
#  endif
    }

    // Oculus Rift �̃Z�b�V������j������
    ovr_Destroy(session);
    session = nullptr;
#endif

    // �E�B���h�E��j������
    glfwDestroyWindow(window);
  }

#if defined(USE_OCULUS_RIFT)
  //
  // Oculus Rift �ɂ��`��J�n
  //
  bool start()
  {
#  if OVR_PRODUCT_VERSION > 0
    // �Z�b�V�����̏�Ԃ��擾����
    ovrSessionStatus sessionStatus;
    ovr_GetSessionStatus(session, &sessionStatus);

    // �A�v���P�[�V�������I����v�����Ă���Ƃ��̓E�B���h�E�̃N���[�Y�t���O�𗧂Ă�
    if (sessionStatus.ShouldQuit) glfwSetWindowShouldClose(window, GL_TRUE);

    // Oculus Rift �ɕ\������Ă��Ȃ��Ƃ��͖߂�
    if (!sessionStatus.IsVisible) return false;

    // ���݂̏�Ԃ��g���b�L���O�̌��_�ɂ���
    if (sessionStatus.ShouldRecenter) ovr_RecenterTrackingOrigin(session);

    // HmdToEyeOffset �Ȃǂ͎��s���ɕω�����̂Ŗ��t���[�� ovr_GetRenderDesc() �� ovrEyeRenderDesc ���擾����
    const ovrEyeRenderDesc eyeRenderDesc[] =
    {
      ovr_GetRenderDesc(session, ovrEyeType(0), hmdDesc.DefaultEyeFov[0]),
      ovr_GetRenderDesc(session, ovrEyeType(1), hmdDesc.DefaultEyeFov[1])
    };

    // Oculus Rift �̃X�N���[���̃w�b�h�g���b�L���O�ʒu����̕ψʂ��擾����
    const ovrVector3f hmdToEyeOffset[] =
    {
      eyeRenderDesc[0].HmdToEyeOffset,
      eyeRenderDesc[1].HmdToEyeOffset
    };

    // Oculus Rift �̃X�N���[���̃w�b�h�g���b�L���O�ʒu����̕ψʂ�ۑ�����
    for (int eye = 0; eye < ovrEye_Count; ++eye)
    {
      offset[eye][0] = hmdToEyeOffset[eye].x;
      offset[eye][1] = hmdToEyeOffset[eye].y;
      offset[eye][2] = hmdToEyeOffset[eye].z;
    }

    // ���_�̎p�������擾����
    ovr_GetEyePoses(session, ++frameIndex, ovrTrue, hmdToEyeOffset, layerData.RenderPose, &layerData.SensorSampleTime);

#  else

    // �t���[���̃^�C�~���O�v���J�n
    const auto ftiming(ovr_GetPredictedDisplayTime(session, 0));

    // sensorSampleTime �̎擾�͉\�Ȍ��� ovr_GetTrackingState() �̋߂��ōs��
    layerData.EyeFov.SensorSampleTime = ovr_GetTimeInSeconds();

    // �w�b�h�g���b�L���O�̏�Ԃ��擾����
    const auto hmdState(ovr_GetTrackingState(session, ftiming, ovrTrue));

    // Oculus Rift �̃X�N���[���̃w�b�h�g���b�L���O�ʒu����̕ψʂ��擾����
    const ovrVector3f hmdToEyeViewOffset[] =
    {
      eyeRenderDesc[0].HmdToEyeViewOffset,
      eyeRenderDesc[1].HmdToEyeViewOffset
    };

    // ���_�̎p���������߂�
    ovr_CalcEyePoses(hmdState.HeadPose.ThePose, hmdToEyeViewOffset, eyePose);
#  endif

    return true;
  }

  //
  // Oculus Rift �̕`�悷��ڂ̎w��
  //
  void select(int eye, GLfloat *screen, GLfloat *position, GLfloat *orientation)
  {
#  if OVR_PRODUCT_VERSION > 0
    // Oculus Rift �Ƀ����_�����O���� FBO �ɐ؂�ւ���
    if (layerData.ColorTexture[eye])
    {
      // FBO �̃J���[�o�b�t�@�Ɏg�����݂̃e�N�X�`���̃C���f�b�N�X���擾����
      int curIndex;
      ovr_GetTextureSwapChainCurrentIndex(session, layerData.ColorTexture[eye], &curIndex);

      // FBO �̃J���[�o�b�t�@�Ɏg���e�N�X�`�����擾����
      GLuint curTexId;
      ovr_GetTextureSwapChainBufferGL(session, layerData.ColorTexture[eye], curIndex, &curTexId);

      // FBO ��ݒ肷��
      glBindFramebuffer(GL_FRAMEBUFFER, oculusFbo[eye]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, curTexId, 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, oculusDepth[eye], 0);

      // �r���[�|�[�g��ݒ肷��
      const auto &vp(layerData.Viewport[eye]);
      glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);
    }

    // Oculus Rift �̕Жڂ̈ʒu�Ɖ�]���擾����
    const auto &p(layerData.RenderPose[eye].Position);
    const auto &o(layerData.RenderPose[eye].Orientation);

#  else

    // �����_�[�^�[�Q�b�g�ɕ`�悷��O�Ƀ����_�[�^�[�Q�b�g�̃C���f�b�N�X���C���N�������g����
    auto *const colorTexture(layerData.EyeFov.ColorTexture[eye]);
    colorTexture->CurrentIndex = (colorTexture->CurrentIndex + 1) % colorTexture->TextureCount;
    auto *const depthTexture(layerData.EyeFovDepth.DepthTexture[eye]);
    depthTexture->CurrentIndex = (depthTexture->CurrentIndex + 1) % depthTexture->TextureCount;

    // �����_�[�^�[�Q�b�g��؂�ւ���
    glBindFramebuffer(GL_FRAMEBUFFER, oculusFbo[eye]);
    const auto &ctex(reinterpret_cast<ovrGLTexture *>(&colorTexture->Textures[colorTexture->CurrentIndex]));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ctex->OGL.TexId, 0);
    const auto &dtex(reinterpret_cast<ovrGLTexture *>(&depthTexture->Textures[depthTexture->CurrentIndex]));
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex->OGL.TexId, 0);

    // �r���[�|�[�g��ݒ肷��
    const auto &vp(layerData.EyeFov.Viewport[eye]);
    glViewport(vp.Pos.x, vp.Pos.y, vp.Size.w, vp.Size.h);

    // Oculus Rift �̕Жڂ̈ʒu�Ɖ�]���擾����
    const auto &p(eyePose[eye].Position);
    const auto &o(eyePose[eye].Orientation);
#  endif

    // Oculus Rift �̃X�N���[���̑傫����Ԃ�
    screen[0] = this->screen[eye][0];
    screen[1] = this->screen[eye][1];
    screen[2] = this->screen[eye][2];
    screen[3] = this->screen[eye][3];

    // Oculus Rift �̈ʒu��Ԃ�
    position[0] = offset[eye][0] + p.x;
    position[1] = offset[eye][1] + p.y;
    position[2] = offset[eye][2] + p.z;
    position[3] = 1.0f;

    // Oculus Rift �̕�����Ԃ�
    orientation[0] = o.x;
    orientation[1] = o.y;
    orientation[2] = o.z;
    orientation[3] = o.w;
  }

  //
  // �}�`�̕`�����������
  //
  void Window::commit(int eye)
  {
#if OVR_PRODUCT_VERSION > 0
    // GL_COLOR_ATTACHMENT0 �Ɋ��蓖�Ă�ꂽ�e�N�X�`���� wglDXUnlockObjectsNV() �ɂ����
    // �A�����b�N����邽�߂Ɏ��̃t���[���̏����ɂ����Ė����� GL_COLOR_ATTACHMENT0 ��
    // FBO �Ɍ��������̂������
    glBindFramebuffer(GL_FRAMEBUFFER, oculusFbo[eye]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);

    // �ۗ����̕ύX�� layerData.ColorTexture[eye] �ɔ��f���C���f�b�N�X���X�V����
    ovr_CommitTextureSwapChain(session, layerData.ColorTexture[eye]);
#endif
  }

  //
  // Time Warp �����Ɏg�����e�ϊ��s��̐����̐ݒ�
  //
  void timewarp(const GgMatrix &projection)
  {
    // TimeWarp �Ɏg���ϊ��s��̐�����ݒ肷��
#  if OVR_PRODUCT_VERSION < 1
    auto &posTimewarpProjectionDesc(layerData.EyeFovDepth.ProjectionDesc);
    posTimewarpProjectionDesc.Projection22 = (projection.get()[4 * 2 + 2] + projection.get()[4 * 3 + 2]) * 0.5f;
    posTimewarpProjectionDesc.Projection23 = projection.get()[4 * 2 + 3] * 0.5f;
    posTimewarpProjectionDesc.Projection32 = projection.get()[4 * 3 + 2];
#  endif
  }
#endif

  //
  // �E�B���h�E�̎��ʎq�̎擾
  //
  GLFWwindow *get() const
  {
    return window;
  }

  //
  // �E�B���h�E�����ׂ����𔻒肷��
  //
  bool shouldClose()
  {
    // �E�B���h�E����邩 ESC �L�[���^�C�v����Ă���ΐ^��Ԃ�
    return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
  }

  //
  // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
  //
  void swapBuffers()
  {
    // �G���[�`�F�b�N
    ggError(__FILE__, __LINE__);

#if defined(USE_OCULUS_RIFT)
#  if OVR_PRODUCT_VERSION > 0
    // �`��f�[�^�� Oculus Rift �ɓ]������
    const auto *const layers(&layerData.Header);
    if (OVR_FAILURE(ovr_SubmitFrame(session, frameIndex, nullptr, &layers, 1)))
#  else
    // Oculus Rift ��̕`��ʒu�Ɗg�嗦�����߂�
    ovrViewScaleDesc viewScaleDesc;
    viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
    viewScaleDesc.HmdToEyeViewOffset[0] = eyeRenderDesc[0].HmdToEyeViewOffset;
    viewScaleDesc.HmdToEyeViewOffset[1] = eyeRenderDesc[1].HmdToEyeViewOffset;

    // �`��f�[�^���X�V����
    layerData.EyeFov.RenderPose[0] = eyePose[0];
    layerData.EyeFov.RenderPose[1] = eyePose[1];

    // �`��f�[�^�� Oculus Rift �ɓ]������
    const auto *const layers(&layerData.Header);
    if (OVR_FAILURE(ovr_SubmitFrame(session, 0, &viewScaleDesc, &layers, 1)))
#  endif
    {
      // �]���Ɏ��s������ Oculus Rift �̐ݒ���ŏ������蒼���K�v������炵��
      // ���ǂ߂�ǂ������̂ŃE�B���h�E����Ă��܂�
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // �����_�����O���ʂ��~���[�\���p�̃t���[���o�b�t�@�ɂ��]������
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
#  if OVR_PRODUCT_VERSION > 0
    const auto w(mirrorWidth), h(mirrorHeight);
#  else
    const auto w(mirrorTexture->OGL.Header.TextureSize.w);
    const auto h(mirrorTexture->OGL.Header.TextureSize.h);
#  endif
    glBlitFramebuffer(0, h, w, 0, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    // �c���Ă��� OpenGL �R�}���h�����s����
    glFlush();

#else

    // �J���[�o�b�t�@�����ւ���
    glfwSwapBuffers(window);
#endif

    // �V�t�g�L�[�ƃR���g���[���L�[�̏�Ԃ����Z�b�g����
    shift_key = control_key = false;

    // �C�x���g�����o��
    glfwPollEvents();

    // �}�E�X�̈ʒu�𒲂ׂ�
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
    const GLfloat x(static_cast<GLfloat>(mouse_x));
    const GLfloat y(static_cast<GLfloat>(mouse_y));

    // ���{�^���h���b�O
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) trackball_left.motion(x, y);

    // �E�{�^���h���b�O
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) trackball_right.motion(x, y);
  }

  //
  // �E�B���h�E�̃T�C�Y�ύX���̏���
  //
  static void resize(GLFWwindow *window, int width, int height)
  {
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance)
    {
      // �E�B���h�E�̃T�C�Y��ۑ�����
      instance->width = width;
      instance->height = height;

      // �g���b�N�{�[�������͈̔͂�ݒ肷��
      instance->trackball_left.region(width, height);
      instance->trackball_right.region(width, height);

#if !defined(USE_OCULUS_RIFT)
      // �E�B���h�E�̃A�X�y�N�g���ۑ�����
      instance->aspect = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);

      // �E�B���h�E�S�̂ɕ`�悷��
      glViewport(0, 0, width, height);
#endif
    }
  }

  //
  // �L�[�{�[�h���^�C�v�������̏���
  //
  static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
  {
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance)
    {
      if (action == GLFW_PRESS)
      {
        switch (key)
        {
        case GLFW_KEY_R:
          // �}�E�X�z�C�[���̉�]�ʂ����Z�b�g����
          instance->wheel_rotation = 0.0;
          instance->shift_wheel_rotation = 0.0;
          instance->control_wheel_rotation = 0.0;

          // ���L�[�̐ݒ�l�����Z�b�g����
          instance->arrow[0] = instance->arrow[1] = 0;
          instance->shift_arrow[0] = instance->shift_arrow[1] = 0;
          instance->control_arrow[0] = instance->control_arrow[1] = 0;

        case GLFW_KEY_O:
          // �g���b�N�{�[�������Z�b�g����
          instance->trackball_left.reset();
          instance->trackball_right.reset();
          break;

        case GLFW_KEY_SPACE:
          break;

        case GLFW_KEY_BACKSPACE:
        case GLFW_KEY_DELETE:
          break;

        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
          instance->shift_key = true;
          break;

        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
          instance->control_key = true;
          break;

        case GLFW_KEY_UP:
          if (instance->shift_key)
            instance->shift_arrow[1]++;
          else if (instance->control_key)
            instance->control_arrow[1]++;
          else
            instance->arrow[1]++;
          break;

        case GLFW_KEY_DOWN:
          if (instance->shift_key)
            instance->shift_arrow[1]--;
          else if (instance->control_key)
            instance->control_arrow[1]--;
          else
            instance->arrow[1]--;
          break;

        case GLFW_KEY_RIGHT:
          if (instance->shift_key)
            instance->shift_arrow[0]++;
          else if (instance->control_key)
            instance->control_arrow[0]++;
          else
            instance->arrow[0]++;
          break;

        case GLFW_KEY_LEFT:
          if (instance->shift_key)
            instance->shift_arrow[0]--;
          else if (instance->control_key)
            instance->control_arrow[0]--;
          else
            instance->arrow[0]--;
          break;

        default:
          break;
        }
      }
    }
  }

  //
  // �}�E�X�{�^���𑀍삵���Ƃ��̏���
  //
  static void mouse(GLFWwindow *window, int button, int action, int mods)
  {
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance)
    {
      // �}�E�X�̌��݈ʒu�𓾂�
      const GLfloat x(static_cast<GLfloat>(instance->mouse_x));
      const GLfloat y(static_cast<GLfloat>(instance->mouse_y));

      switch (button)
      {
      case GLFW_MOUSE_BUTTON_1:
        if (action)
        {
          // ���h���b�O�J�n
          instance->trackball_left.start(x, y);
        }
        else
        {
          // ���h���b�O�I��
          instance->trackball_left.stop(x, y);
        }
        break;

      case GLFW_MOUSE_BUTTON_2:
        if (action)
        {
          // �E�h���b�O�J�n
          instance->trackball_right.start(x, y);
        }
        else
        {
          // �E�h���b�O�I��
          instance->trackball_right.stop(x, y);
        }
        break;

      case GLFW_MOUSE_BUTTON_3:
        break;

      default:
        break;
      }
    }
  }

  //
  // �}�E�X�z�C�[���𑀍삵�����̏���
  //
  static void wheel(GLFWwindow *window, double x, double y)
  {
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));

    if (instance)
    {
      if (instance->shift_key)
        instance->shift_wheel_rotation += y;
      else if (instance->control_key)
        instance->control_wheel_rotation += y;
      else
      {
        instance->wheel_rotation += y;
        if (instance->wheel_rotation < -100.0)
          instance->wheel_rotation = -100.0;
        else if (instance->wheel_rotation > 49.0)
          instance->wheel_rotation = 49.0;
      }
    }
  }

  //
  // �E�B���h�E�̕��𓾂�
  //
  GLsizei getWidth() const
  {
    return width;
  }

  //
  // �E�B���h�E�̍����𓾂�
  //
  GLsizei getHeight() const
  {
    return height;
  }

  //
  // �E�B���h�E�̃T�C�Y�𓾂�
  //
  void getSize(GLsizei *size) const
  {
    size[0] = getWidth();
    size[1] = getHeight();
  }

  //
  // �E�B���h�E�̃A�X�y�N�g��𓾂�
  //
  GLfloat getAspect() const
  {
    return aspect;
  }

  //
  // ���L�[�̌��݂� X �l�𓾂�
  //
  GLfloat getArrowX() const
  {
    return static_cast<GLfloat>(arrow[0]);
  }

  //
  // ���L�[�̌��݂� Y �l�𓾂�
  //
  GLfloat getArrowY() const
  {
    return static_cast<GLfloat>(arrow[1]);
  }

  //
  // ���L�[�̌��݂̒l�𓾂�
  //
  void getArrow(GLfloat *arrow) const
  {
    arrow[0] = getArrowX();
    arrow[1] = getArrowY();
  }

  //
  // �V�t�g�L�[�������Ȃ�����L�[�̌��݂� X �l�𓾂�
  //
  GLfloat getShiftArrowX() const
  {
    return static_cast<GLfloat>(shift_arrow[0]);
  }

  //
  // �V�t�g�L�[�������Ȃ�����L�[�̌��݂� Y �l�𓾂�
  //
  GLfloat getShiftArrowY() const
  {
    return static_cast<GLfloat>(shift_arrow[1]);
  }

  //
  // �V�t�g�L�[�������Ȃ�����L�[�̌��݂̒l�𓾂�
  //
  void getShiftArrow(GLfloat *shift_arrow) const
  {
    shift_arrow[0] = getShiftArrowX();
    shift_arrow[1] = getShiftArrowY();
  }

  //
  // �R���g���[���L�[�������Ȃ�����L�[�̌��݂� X �l�𓾂�
  //
  GLfloat getControlArrowX() const
  {
    return static_cast<GLfloat>(control_arrow[0]);
  }

  //
  // �R���g���[���L�[�������Ȃ�����L�[�̌��݂� Y �l�𓾂�
  //
  GLfloat getControlArrowY() const
  {
    return static_cast<GLfloat>(control_arrow[1]);
  }

  //
  // �R���g���[���L�[�������Ȃ�����L�[�̌��݂̒l�𓾂�
  //
  void getControlArrow(GLfloat *control_arrow) const
  {
    control_arrow[0] = getControlArrowX();
    control_arrow[1] = getControlArrowY();
  }

  //
  // �}�E�X�� X ���W�𓾂�
  //
  GLfloat getMouseX() const
  {
    return static_cast<GLfloat>(mouse_x);
  }

  //
  // �}�E�X�� Y ���W�𓾂�
  //
  GLfloat getMouseY() const
  {
    return static_cast<GLfloat>(mouse_y);
  }

  //
  // �}�E�X�̌��݈ʒu�𓾂�
  //
  void getMouse(GLfloat *position) const
  {
    position[0] = getMouseX();
    position[1] = getMouseY();
  }

  //
  // �}�E�X�z�C�[���̌��݂̉�]�p�𓾂�
  //
  GLfloat getWheel() const
  {
    return static_cast<GLfloat>(wheel_rotation);
  }

  //
  // �V�t�g�������Ȃ���}�E�X�z�C�[���̌��݂̉�]�p�𓾂�
  //
  GLfloat getShiftWheel() const
  {
    return static_cast<GLfloat>(shift_wheel_rotation);
  }

  //
  // �R���g���[���������Ȃ���}�E�X�z�C�[���̌��݂̉�]�p�𓾂�
  //
  GLfloat getControlWheel() const
  {
    return static_cast<GLfloat>(control_wheel_rotation);
  }

  //
  // ���{�^���ɂ��g���b�N�{�[���̉�]�ϊ��s��𓾂�
  //
  const GgMatrix getLeftTrackball() const
  {
    return trackball_left.getMatrix();
  }

  //
  // �E�{�^���ɂ��g���b�N�{�[���̉�]�ϊ��s��𓾂�
  //
  const GgMatrix getRightTrackball() const
  {
    return trackball_right.getMatrix();
  }
};
