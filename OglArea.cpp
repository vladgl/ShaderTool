#include "OglArea.h"
#include <iostream>
#include <QDebug>
#include "Shaders.h"
#include <QMouseEvent>
#include <QTextBlock>
OglArea::OglArea()
{
    //this property tells widget to accept keybord focus both by tabbing and clicking
    this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    this->setMouseTracking(true);
}
OglArea::~OglArea()
{

}

void OglArea::initializeGL()
{
    this->initializeOpenGLFunctions();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vertexSource = VERTEX_SHADER;

    const char** fragmentSourceArray = new const char* [3];

    fragmentSourceArray[0] = FRAGMENT_SHADER_HEADER;
    fragmentSourceArray[1] = MAIN_IMAGE_BASIC_DEF;
    fragmentSourceArray[2] = FRAGMENT_MAIN;

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 3, fragmentSourceArray, nullptr);

    delete [] fragmentSourceArray;

    glCompileShader(vertexShader);

    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        qDebug() << infoLog;
    }
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        qDebug() << infoLog;
    }


    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        qDebug() << infoLog;
    }

    GLfloat tri[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,

        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };


    glGenBuffers(1, &buff);
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    connect(&timer, &QTimer::timeout, this, static_cast<void (OglArea::*)()>(&OglArea::update));
    timer.start(0);

    renderStart = sht::timer::now();
    renderPass = sht::timer::now();
    frame = 1;
}
void OglArea::paintGL()
{
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_CULL_FACE);

    glUseProgram(shaderProgram);

    glBindBuffer(GL_ARRAY_BUFFER, buff);
    GLint pos_pos = glGetAttribLocation(shaderProgram, "st_Position");
    if(pos_pos >= 0)
    {
        GLint i_pos;

        float time = sht::duration_cast<sht::milliseconds>(sht::timer::now() - renderStart).count() / 1000.0f;
        i_pos = glGetUniformLocation(shaderProgram, "iTime");
        glUniform1f(i_pos, time);

        float dtime = sht::duration_cast<sht::milliseconds>(sht::timer::now() - renderPass).count() / 1000.0f;
        i_pos = glGetUniformLocation(shaderProgram, "iTimeDelta");
        glUniform1f(i_pos, dtime);

        i_pos = glGetUniformLocation(shaderProgram, "iFrame");
        glUniform1i(i_pos, frame);

        renderPass = sht::timer::now();
        glVertexAttribPointer(pos_pos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)nullptr);
        glEnableVertexAttribArray(pos_pos);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);
    glErrorHandler();
    frame++;
}
void OglArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    GLint i_pos = glGetUniformLocation(shaderProgram, "iResolution");
    glUseProgram(shaderProgram);
    glUniform2f(i_pos, (float)w, (float)h);
    glUseProgram(0);
}

void OglArea::mouseMoveEvent(QMouseEvent *event)
{
    this->makeCurrent();
    GLint i_pos = glGetUniformLocation(shaderProgram, "iMouse");
    float left = event->buttons() & Qt::LeftButton;
    float right = event->buttons() & Qt::RightButton;
    glUseProgram(shaderProgram);
    glUniform4f(i_pos, event->pos().x(), height() - event->pos().y(), left, right);
    glUseProgram(0);
}


void OglArea::compileSource(QString const& src)
{
    bool failed = false;
    QString error_str;

    this->makeCurrent();
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(fragmentShader);

    const char** fragmentSourceArray = new const char* [3];

    fragmentSourceArray[0] = FRAGMENT_SHADER_HEADER;
    auto tmp = src.toLocal8Bit();
    fragmentSourceArray[1] = tmp.data();
    fragmentSourceArray[2] = FRAGMENT_MAIN;

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 3, fragmentSourceArray, nullptr);

    delete [] fragmentSourceArray;

    glCompileShader(fragmentShader);
    GLint status;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

    GLchar infoLog[512];
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

    if(status != GL_TRUE)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        failed = true;
        error_str += QString(infoLog) + '\n';
    }


    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        failed = true;
        error_str += QString(infoLog) + '\n';
    }

    if(failed)
    {
        emit sourceFailedToCompile(error_str);
        return;
    }

    glUseProgram(shaderProgram);

    GLint res_pos = glGetUniformLocation(shaderProgram, "iResolution");
    GLint mou_pos = glGetUniformLocation(shaderProgram, "iMouse");

    glUseProgram(shaderProgram);
    glUniform2f(res_pos, (float)width(), (float)height());
    glUniform4f(mou_pos, width() / 2.0f, height() / 2.0f, 0.0, 0.0);
    glUseProgram(0);

    renderStart = sht::timer::now();
    renderPass = sht::timer::now();
    frame = 1;
    emit sourceCompiled();
    glErrorHandler();
}



void OglArea::glErrorHandler()
{
    this->makeCurrent();
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        switch (err)
        {
        case GL_INVALID_ENUM:
            qDebug() << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
             qDebug() << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
             qDebug() << "GL_INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
             qDebug() << "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
             qDebug() << "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
             qDebug() << "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            qDebug() << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_CONTEXT_LOST:
             qDebug() << "GL_CONTEXT_LOST";
            break;
        default:
             qDebug() << "Unknown error";
            break;
        }
}

