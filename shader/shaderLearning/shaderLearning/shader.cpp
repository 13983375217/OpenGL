#include "shader.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
};

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //这里我们检查用户是否按下了返回键(Esc)
}
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

    Shader ourShader("shader.vs", "shader.fs");



    //如果要绘制两个三角形就需要使用索引缓存对象来告诉GPU使用哪些顶点
    unsigned int VAO, VBO;
    unsigned int EBO;
    glGenBuffers(1, &EBO);//glGenBuffers创建索引对象

    glGenVertexArrays(1, &VAO);//glGenVertexArrays来创建顶点数组
    glGenBuffers(1, &VBO);//glGenBuffers来创建缓存

    glBindVertexArray(VAO);//为下面的属性绑定VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER。
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//绑定顶点并把顶点发送给GPU 第三个参数就是我们希望发送的实际数据。

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定EBO
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//如何使用索引

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);//GPU有了顶点之后使用这个函数来链接顶点属性glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));//GPU有了顶点之后使用这个函数来链接顶点属性glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑V
    glBindVertexArray(0);//使用完成之后解绑就行了为下面的属性绑定VAO


    //循环直到用户关闭窗口 渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//使用按键

        //清理屏幕所用的颜色：
        glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

        //清理屏幕  它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，
        //可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//使用glPolygonMode设置绘制模式这里是绘制线框
        ourShader.use();//使用这个程序
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //交换前后缓冲
        glfwSwapBuffers(window);

        //轮询并处理事件
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);



    //使用GLFW完成操作后，通常是在应用程序退出之前，需要终止GLFW 释放/删除之前的分配的所有资源
    glfwTerminate();

    return 0;
}