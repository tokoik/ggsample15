#include <cmath>
#include <cstdlib>

#include "gg.h"

/*
** ��ʕ\��
*/
static void display(void)
{
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT);

  glutSwapBuffers();
}

/*
** �E�B���h�E�̃��T�C�Y
*/
static void resize(int w, int h)
{
  // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
  glViewport(0, 0, w, h);
}

/*
** ������
*/
static void init(void)
{
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  gg::ggInit();
  
  // �w�i�F
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

/*
** ���C���v���O����
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("ggsample15");
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  init();
  glutMainLoop();

  return 0;
}
