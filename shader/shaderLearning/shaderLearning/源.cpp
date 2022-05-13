#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
using namespace std;
void processInput(GLFWwindow* window)//����������������û���������
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //�������Ǽ���û��Ƿ����˷��ؼ�(Esc)
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
    //�����ȳ�ʼ���ÿ⣬Ȼ�����ʹ�ô����GLFW�������ɹ���ʼ����GLFW_TRUE�����ء������������GLFW_FALSE�򷵻ء�
    if (!glfwInit())
        return -1;

    //�������ڣ�OpenGL�������ƺ�Ҳһ�������ˣ�
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    //glfwCreateWindow������Ҫ���ڵĿ�͸���Ϊ����ǰ����������������������ʾ������ڵ����ƣ�����

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //ʹ��GLAD������OpenGL�ĺ�����ַ GLAD����������OpenGL�ĺ���ָ��ģ������ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD��
    //���Ǹ�GLAD��������������ϵͳ��ص�OpenGL����ָ���ַ�ĺ�����GLFW�����ǵ���glfwGetProcAddress�����������Ǳ����ϵͳ��������ȷ�ĺ�����
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    unsigned int vertexShader;//����һ������vertexShader ��ID����������ʹ�� unsigned int��
    vertexShader = glCreateShader(GL_VERTEX_SHADER);//glCreateShader������shader �Բ�����ʽ���ߺ���GL_VERTEX_SHADER ������ɫ��
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);//��һ�����ǰ������ɫ��Դ�븽�ӵ���ɫ�������ϣ�Ȼ���������
    glCompileShader(vertexShader);
    /*
    glShaderSource������Ҫ�������ɫ��������Ϊ��һ���������ڶ�����ָ���˴��ݵ�Դ���ַ���������
    ����ֻ��һ���������������Ƕ�����ɫ��������Դ�룬���ĸ���������������ΪNULL��
    */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    /*
   ���Ƭ����ɫ���Ͷ�����ɫ��һ��
   */


   //��Դ�����֮����Ҫ������Դ�����ӵ�һ����������
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();//ʹ�����glCreateProgram����һ������ ����ID������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader); //������Դ�븽�ӵ�������
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader); //�������֮��ɾ�������ͷ��ڴ�
    glDeleteShader(fragmentShader);


    /*
    VBO���㻺����� ����Ϊ�����ڴ� ����������GPU�ϴ����ڴ����ڴ������ǵĶ������ݵ��ڴ�
    ����һ��VBO����  �ѱ�������glGenBuffers�������ɺ����������һ�������һ��ID
    Ȼ���������������������GL_ARRAY_BUFFER
    */
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);//glGenVertexArrays��������������
    glGenBuffers(1, &VBO);//glGenBuffers����������
    glBindVertexArray(VAO);//Ϊ��������԰�VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER��
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�󶨶��㲢�Ѷ��㷢�͸�GPU ������������������ϣ�����͵�ʵ�����ݡ�

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //ѭ��ֱ���û��رմ��� ��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ��������
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//ʹ�ð���

        //������Ļ���õ���ɫ��
        glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

        //������Ļ  ������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ��壬
        //���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT��������������ֻ������ɫֵ����������ֻ�����ɫ���塣
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);//ʹ���������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //����ǰ�󻺳�
        glfwSwapBuffers(window);

        //��ѯ�������¼�
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //ʹ��GLFW��ɲ�����ͨ������Ӧ�ó����˳�֮ǰ����Ҫ��ֹGLFW �ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();

    return 0;
}
