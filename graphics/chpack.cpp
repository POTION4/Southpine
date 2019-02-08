//
//  chpack.cpp
//  Southpine
//
//  Created by 周昊 on 2019/2/6.
//  Copyright © 2019 aiofwa. All rights reserved.
//


#include "chpack.hpp"
#include "utility.hpp"
#include <map>
#include <iostream>
#include "../memory/bus.hpp"


CHPack::CHPack(int x, int y) {
    this->pos = { 0.0f, 0.0f, 0.0f };
    this->VAO = genRect();
    
    // back means x * width, y * height
    // right means ...
    // front means ...
    this->botX = x * bus().texWidth + 1;
    this->botY = y * bus().texHeight + 1;
    
    float buffer[] = {
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void *) (sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void *) (sizeof(float) * 6));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void *) (sizeof(float) * 9));
    
    this->x = x;
    this->y = y;
}



void CHPack::draw(GLuint prog) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, this->pos);
    
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    float *memMap = (float *) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    memMap[9] = this->botX;
    memMap[10] = this->botY;
    memMap[20] = this->botX + bus().texWidth;
    memMap[21] = this->botY;
    memMap[31] = this->botX;
    memMap[32] = this->botY + bus().texHeight;
    memMap[42] = this->botX;
    memMap[43] = this->botY + bus().texHeight;
    memMap[53] = this->botX + bus().texWidth;
    memMap[54] = this->botY + bus().texHeight;
    memMap[64] = this->botX + bus().texWidth;
    memMap[65] = this->botY;
    
    for (int i = 0; i < 6; i++) {
        memMap[i * 11 + 9] /= bus().sheetWidth;
        memMap[i * 11 + 10] /= bus().sheetHeight;
    }
    
    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    
    glUseProgram(prog);
    
    glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, value_ptr(model));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, bus().chSheet);
    glUniform1i(glGetUniformLocation(prog, "tex"), 0);
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

