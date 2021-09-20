//
// ゲームグラフィックス特論宿題アプリケーション
//
#include "GgApp.h"

//
// アプリケーション本体
//
int GgApp::main(int argc, const char* const* argv)
{
  // ウィンドウを作成する
  Window window{ "ggsample15" };

  // シェーダのプログラムオブジェクトを作成する
  const GLuint shader{ ggLoadShader("simple.vert", "simple.frag") };

  // 頂点配列オブジェクトを作成する
  GLuint vao;
  glGenVertexArrays(1, &vao);

  // 頂点配列オブジェクトを結合する
  glBindVertexArray(vao);

  // 頂点バッファオブジェクトを作成する
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // 作成した頂点バッファオブジェクトを結合する
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // 座標データ
  static const GLfloat position[][2]{
    { -0.5f, -0.5f },
    {  0.5f, -0.5f },
    {  0.5f,  0.5f },
    { -0.5f,  0.5f }
  };

  // 頂点バッファオブジェクトのメモリを確保してデータを座標データを転送する
  glBufferData(GL_ARRAY_BUFFER, sizeof position, position, GL_STATIC_DRAW);

  // この頂点バッファオブジェクトの先頭に 0 番のインデックスを割り当てる
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // 0 番のインデックスを有効にする
  glEnableVertexAttribArray(0);

  // 頂点バッファオブジェクトの結合を解除する
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 頂点配列オブジェクトの結合を解除する
  glBindVertexArray(0);

  // 背景色を指定する
  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // ウィンドウが開いている間繰り返す
  while (window)
  {
    // ウィンドウ内の表示を消去する
    glClear(GL_COLOR_BUFFER_BIT);

    // 描画に使用するシェーダを指定する
    glUseProgram(shader);

    // 描画する頂点配列オブジェクトを指定する
    glBindVertexArray(vao);

    // 図形を描画する
    glDrawArrays(GL_LINE_STRIP, 0, 4);

    // 頂点配列オブジェクトの指定を解除する
    glBindVertexArray(0);

    // シェーダの指定を解除する
    glUseProgram(0);

    // カラーバッファを入れ替えてイベントを取り出す
    window.swapBuffers();
  }

  return 0;
}
