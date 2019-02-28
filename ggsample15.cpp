﻿// アプリケーション本体
#include "GgApplication.h"

//
// アプリケーションの実行
//
void GgApplication::run()
{
  // ウィンドウを作成する
  Window window("ggsample01");

  // 背景色を指定する
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウを消去する
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // ここで OpenGL による描画を行う
    //

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }
}
