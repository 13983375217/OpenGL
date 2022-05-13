#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
using namespace std;
void processInput(GLFWwindow* window)//用这个函数来控制用户键盘输入
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //这里我们检查用户是否按下了返回键(Esc)
}

float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
};


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

int main(void)
{
    //必须先初始化该库，然后才能使用大多数GLFW函数。成功初始化后，GLFW_TRUE将返回。如果发生错误，GLFW_FALSE则返回。
    if (!glfwInit())
        return -1;

    //创建窗口（OpenGL上下文似乎也一并创建了）
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    //glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //使用GLAD来加载OpenGL的函数地址 GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    //我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    unsigned int vertexShader;//创建一个对象vertexShader 以ID来索引所以使用 unsigned int型
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//glCreateShader来创建shader 以参数形式告诉函数GL_VERTEX_SHADER 顶点做色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//下一步我们把这个着色器源码附加到着色器对象上，然后编译它：
    glCompileShader(vertexShader);
    /*
    glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，
    这里只有一个。第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
    */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    /*
   这个片段着色器和定点着色器一样
   */


   //将源码编译之后需要把两个源码链接到一个程序里面
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//使用这个glCreateProgram创建一个程序 返回ID的索引
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader); //把两个源码附加到程序上
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader); //调用完成之后删除对象释放内存
    glDeleteShader(fragmentShader);


    /*
    VBO顶点缓冲对象 作用为管理内存 用来管理在GPU上创建内存用于储存我们的顶点数据的内存
    定义一个VBO变量  把变量传入glGenBuffers缓冲生成函数里面产生一个缓冲和一个ID
    然后把这个缓冲对象用来管理GL_ARRAY_BUFFER
    */
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);//glGenVertexArrays来创建顶点数组
    glGenBuffers(1, &VBO);//glGenBuffers来创建缓存
    glBindVertexArray(VAO);//为下面的属性绑定VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//绑定顶点并把顶点发送给GPU 第三个参数就是我们希望发送的实际数据。

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //循环直到用户关闭窗口 渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//使用按键

        //清理屏幕所用的颜色：
        glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

        //清理屏幕  它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，
        //可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);//使用这个程序
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //交换前后缓冲
        glfwSwapBuffers(window);

        //轮询并处理事件
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //使用GLFW完成操作后，通常是在应用程序退出之前，需要终止GLFW 释放/删除之前的分配的所有资源
    glfwTerminate();

    return 0;
}
