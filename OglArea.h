#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QTimer>

namespace sht
{
    typedef std::chrono::steady_clock timer;
    typedef std::chrono::milliseconds milliseconds;
    typedef std::chrono::seconds seconds;
    typedef std::chrono::steady_clock::time_point time_point;

    template<class T, class U>
    T duration_cast(U val)
    {
        return std::chrono::duration_cast<T>(val);
    }
}



class OglArea : public QOpenGLWidget, public QOpenGLFunctions
{
Q_OBJECT
public:
    OglArea();
    ~OglArea();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
public slots:
    void compileSource(QString const& str);
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
private:

    void glErrorHandler();


    QTimer timer;
    sht::time_point renderStart;
    sht::time_point renderPass;
    int frame;

    GLuint shaderProgram;
    GLuint vertexArray;
    GLuint fragmentShader;
    GLuint buff;
};
