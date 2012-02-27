#include <cmath>
#include <cstdlib>

#include "gg.h"

/*
** 画面表示
*/
static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT);

  glutSwapBuffers();
}

/*
** ウィンドウのリサイズ
*/
static void resize(int w, int h)
{
  // ウィンドウ全体をビューポートにする
  glViewport(0, 0, w, h);
}

/*
** 初期化
*/
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  gg::ggInit();
  
  // 背景色
  glClearColor(1.0, 1.0, 1.0, 1.0);
}

/*
** メインプログラム
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
