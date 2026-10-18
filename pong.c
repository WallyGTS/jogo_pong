#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SOM_DA_BOLA "sounds/beachball_bounce01.wav"
#define SOM_DE_GOL "sounds/Sound Effect (6).wav"
const int altura = 500;
const int largura = 1080;

bool pause = false;

typedef struct raquete{
    Vector2 pos;
    Vector2 vel;
    int raltura;
    int largura;
    Color cor;
}raquete;

void desenhar_raquete_esquerda(raquete *Raquete_esquerda){
    DrawRectangle(Raquete_esquerda->pos.x, Raquete_esquerda->pos.y, Raquete_esquerda->largura, Raquete_esquerda->raltura, Raquete_esquerda->cor);
}

void desenhar_raquete_direita(raquete *Raquete_direita){
    DrawRectangle(Raquete_direita->pos.x, Raquete_direita->pos.y, Raquete_direita->largura, Raquete_direita->raltura, Raquete_direita->cor);
}

typedef struct Bolinha {
    Vector2 pos;
    Vector2 vel;
    int raio;
    Color cor;
}Bolinha;

void desenhar(Bolinha *bolinha){
    DrawCircle(bolinha->pos.x, bolinha->pos.y, bolinha->raio, bolinha->cor);
}

void resetar_bolinha(Bolinha *bolinha){
    bolinha->pos.x = largura / 2;
    bolinha->pos.y = altura / 2;
    bolinha->vel.x *=-1;
}
void atualizar(Bolinha *bolinha, int *placar_esquerdo, int *placar_direito, Sound somGol){
    bolinha->pos.x += bolinha->vel.x;
    bolinha->pos.y += bolinha->vel.y;
    
    if (bolinha->pos.x + bolinha->raio >= largura){
        (*placar_esquerdo)++;
        PlaySound(somGol);
    resetar_bolinha(bolinha);
    }
    else if(bolinha->pos.x - bolinha->raio <= 0){
        (*placar_direito)++;
        PlaySound(somGol);
        resetar_bolinha(bolinha);
    }
    if (bolinha->pos.y + bolinha->raio >= altura || bolinha->pos.y - bolinha->raio <= 0){
        bolinha->vel.y *= -1;
    }
    
}
void desenhar_placar(int placar_esquerda, int placar_direita){
       DrawText(TextFormat("%d",placar_esquerda), 0.50 * largura -350, 30, 50, WHITE);
       DrawText(TextFormat("%d",placar_direita), 0.50 * largura +300 , 30, 50, WHITE);
       DrawLine(largura / 2, 0, largura / 2, altura, WHITE);
       if(placar_esquerda == 10){
        DrawText("JOGADOR 1 GANHOU!",0.50 * largura - 280,altura/2 - 20,50, BLUE);
       } else if(placar_direita == 10){
        DrawText("JOGADOR 2 GANHOU!",0.50 * largura - 280,altura/2 - 20,50, BLUE);
       }
}


void colisao(Bolinha *bolinha, raquete *Raquete_esquerda, raquete *Raquete_direita, Sound somBola){
    Rectangle recEsquerda = { Raquete_esquerda->pos.x, Raquete_esquerda->pos.y, Raquete_esquerda->largura, Raquete_esquerda->raltura };
    if (CheckCollisionCircleRec(bolinha->pos, bolinha->raio, recEsquerda)) {
        if (bolinha->vel.x < 0){

         bolinha->vel.x *= -1;
         PlaySound(somBola);
      }
    } 

    Rectangle recDireita = { Raquete_direita->pos.x, Raquete_direita->pos.y, Raquete_direita->largura, Raquete_direita->raltura };
    if (CheckCollisionCircleRec(bolinha->pos, bolinha->raio, recDireita)) {
        if (bolinha->vel.x > 0){
        bolinha->vel.x *= -1;
         PlaySound(somBola);
        } 
    }
}

void atualizar_raquetes(raquete *Raquete_esquerda, raquete *Raquete_direita){
    if(IsKeyDown(KEY_W) && Raquete_esquerda->pos.y > 0) 
        Raquete_esquerda->pos.y -= Raquete_esquerda->vel.y;
    if(IsKeyDown(KEY_S) && Raquete_esquerda->pos.y +Raquete_esquerda->raltura < altura) 
        Raquete_esquerda->pos.y += Raquete_esquerda->vel.y;
        
    if(IsKeyDown(KEY_UP) && Raquete_direita->pos.y > 0) 
        Raquete_direita->pos.y -= Raquete_direita->vel.y;
    if(IsKeyDown(KEY_DOWN) && Raquete_direita->pos.y + Raquete_direita->raltura < altura) 
        Raquete_direita->pos.y += Raquete_direita->vel.y;
}


int main(){
    InitWindow(largura, altura, "PONG");
    InitAudioDevice();
    SetTargetFPS(60);

    int placar_esquerdo = 0;
    int placar_direito = 0;

    Sound somBola = LoadSound(SOM_DA_BOLA);
    Sound somGol = LoadSound(SOM_DE_GOL);

    Bolinha bolinha = {
        .pos = {
            .x = largura / 2,
            .y = altura / 2,
        },
        .vel = {
            .x = 15,
            .y = 15,
        },
        .raio = 25,
        .cor = WHITE
    };

    raquete Raquete_esquerda = {
        .pos = {
            .x = 0.001 * largura,
            .y = (altura/2) - 40,
        },
        .vel = {
            .x = 0,
            .y = 24,
        },
        .raltura = 80,
        .largura = 30,
        .cor = WHITE,
    };

    raquete Raquete_direita = {
        .pos = {
            .x = largura * 0.972,
            .y = (altura/2) - 40,
        },
        .vel = {
            .x = 0,
            .y = 24,
        },
        .raltura = 80,
        .largura = 30,
        .cor = WHITE,
    };


    while(!WindowShouldClose()){
         

        if(IsKeyPressed(KEY_Q))

         pause = !pause;
         if(!pause &&placar_direito < 10 && placar_esquerdo < 10){
        atualizar(&bolinha, &placar_esquerdo, &placar_direito, somGol);
        atualizar_raquetes(&Raquete_esquerda, &Raquete_direita);
        colisao(&bolinha, &Raquete_esquerda, &Raquete_direita, somBola);
         }
    
         BeginDrawing();
        ClearBackground(BLACK);
        if(pause)
        DrawText("JOGO PAUSADO", 375, altura/2, 40, WHITE);
            DrawLine(largura * 0.50, 0, largura * 0.50, altura,WHITE);
            desenhar(&bolinha);
            desenhar_raquete_esquerda(&Raquete_esquerda);
            desenhar_raquete_direita(&Raquete_direita);
            desenhar_placar(placar_esquerdo, placar_direito);
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}