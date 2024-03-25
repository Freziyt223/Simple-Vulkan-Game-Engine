#include <stdio.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    double lastTime;
    int frameCount;
} Timer;

Timer timer;

void initTimer() {
    timer.lastTime = glfwGetTime();
    timer.frameCount = 0;
}

void updateFrameCount() {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - timer.lastTime;
    timer.lastTime = currentTime;
    
    // Оновлюємо лічильник кадрів
    timer.frameCount++;
    
    // Виводимо час між кадрами (deltaTime)
    printf("Delta Time: %f\n", deltaTime);
    
    // Обчислюємо кадри в секунду (FPS)
    if (deltaTime >= 1.0) {
        double fps = (double)timer.frameCount / deltaTime;
        printf("FPS: %.2f\n", fps);
        timer.frameCount = 0;
    }
}

int main() {
    // Ініціалізація GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Створення вікна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Game", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    // Ініціалізація таймера
    initTimer();
    
    // Основний цикл
    while (!glfwWindowShouldClose(window)) {
        // Оновлення часу між кадрами
        updateFrameCount();
        
        // Перевірка подій та обмін буферами
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    // Завершення програми
    glfwTerminate();
    return 0;
}
