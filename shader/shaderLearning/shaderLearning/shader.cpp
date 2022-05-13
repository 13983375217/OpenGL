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
    //�������Ǽ���û��Ƿ����˷��ؼ�(Esc)
}
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

    Shader ourShader("shader.vs", "shader.fs");



    //���Ҫ�������������ξ���Ҫʹ�������������������GPUʹ����Щ����
    unsigned int VAO, VBO;
    unsigned int EBO;
    glGenBuffers(1, &EBO);//glGenBuffers������������

    glGenVertexArrays(1, &VAO);//glGenVertexArrays��������������
    glGenBuffers(1, &VBO);//glGenBuffers����������

    glBindVertexArray(VAO);//Ϊ��������԰�VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //OpenGL�кܶ໺��������ͣ����㻺�����Ļ���������GL_ARRAY_BUFFER��
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�󶨶��㲢�Ѷ��㷢�͸�GPU ������������������ϣ�����͵�ʵ�����ݡ�

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//��EBO
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//���ʹ������

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);//GPU���˶���֮��ʹ��������������Ӷ�������glVertexAttribPointer��������OpenGL����ν�����������
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));//GPU���˶���֮��ʹ��������������Ӷ�������glVertexAttribPointer��������OpenGL����ν�����������
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//���V
    glBindVertexArray(0);//ʹ�����֮���������Ϊ��������԰�VAO


    //ѭ��ֱ���û��رմ��� ��Ⱦѭ��(Render Loop)��������������GLFW�˳�ǰһֱ��������
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//ʹ�ð���

        //������Ļ���õ���ɫ��
        glClearColor(0.4f, 0.5f, 0.6f, 1.0f);

        //������Ļ  ������һ������λ(Buffer Bit)��ָ��Ҫ��յĻ��壬
        //���ܵĻ���λ��GL_COLOR_BUFFER_BIT��GL_DEPTH_BUFFER_BIT��GL_STENCIL_BUFFER_BIT��������������ֻ������ɫֵ����������ֻ�����ɫ���塣
        glClear(GL_COLOR_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//ʹ��glPolygonMode���û���ģʽ�����ǻ����߿�
        ourShader.use();//ʹ���������
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //����ǰ�󻺳�
        glfwSwapBuffers(window);

        //��ѯ�������¼�
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);



    //ʹ��GLFW��ɲ�����ͨ������Ӧ�ó����˳�֮ǰ����Ҫ��ֹGLFW �ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();

    return 0;
}