//
//  Game.cpp
//  SFML_TTT
//
//  Created by Alex on 19.05.2025.
//
#include "Game.h"
#include <iostream>
#include <stdlib.h>


constexpr double GEM_ANIMATION_SPEED = 0.0009;
constexpr int BOMB_ANIMATION_SHAKE_AMPLITUDE = 4;
constexpr int MAIN_BOMB_ADDITIONAL_SIZE = 4;

constexpr int BONUS_CHANCE = 8;

sf::Color color2sfml_color(Color color){
    switch(color){
        case Color::PINK:
            return sf::Color (255, 192, 213);
        case Color::GREEN:
            return sf::Color (112, 130, 56);
        case Color::RED:
            return sf::Color (255, 0, 0);
        case Color::PURPLE:
            return sf::Color (187, 145, 222);
        case Color::YELLOW:
            return sf::Color (255, 197, 36);
        case Color::WHITE:
            return sf::Color (255, 255, 255);
    }
    return sf::Color (255, 255, 255);
}

double max(double a, double b) {
    return a>b ? a : b;
}

double min(double a, double b) {
    return a<b ? a : b;
}

void Gem::paint(pair<int, int> position, sf::RenderWindow& window, int gem_size, int gem_gap) {
    sf::RectangleShape gem({float(gem_size), float(gem_size)});
    gem.setFillColor(color2sfml_color(color));
    
    if (animation_state > 0){
        if (animation_type == Animation_type::MOVE_RIGHT)
            position.first = position.first-animation_state*(gem_size+gem_gap);
        
        if (animation_type == Animation_type::MOVE_LEFT)
            position.first = position.first+animation_state*(gem_size+gem_gap);
        
        if (animation_type == Animation_type::MOVE_UP)
            position.second = position.second+animation_state*(gem_size+gem_gap);
        
        if (animation_type == Animation_type::MOVE_DOWN)
            position.second = position.second-animation_state*(gem_size+gem_gap);
        
        if(animation_type == Animation_type::SECONDARY_BOMB_ANIMATE){
            gem.setFillColor(color2sfml_color(Color::RED));
            position.first = position.first+(BOMB_ANIMATION_SHAKE_AMPLITUDE/2-rand()%BOMB_ANIMATION_SHAKE_AMPLITUDE);
            position.second = position.second+(BOMB_ANIMATION_SHAKE_AMPLITUDE/2-rand()%BOMB_ANIMATION_SHAKE_AMPLITUDE);
        }
        
        if(animation_type == Animation_type::MAIN_BOMB_ANIMATE){
            gem.setFillColor(color2sfml_color(Color::RED));
            gem.setSize({float(gem_size+2*MAIN_BOMB_ADDITIONAL_SIZE), float(gem_size+2*MAIN_BOMB_ADDITIONAL_SIZE)});
            position.first = position.first+(BOMB_ANIMATION_SHAKE_AMPLITUDE/2-rand()%BOMB_ANIMATION_SHAKE_AMPLITUDE) - MAIN_BOMB_ADDITIONAL_SIZE;
            position.second = position.second+(BOMB_ANIMATION_SHAKE_AMPLITUDE/2-rand()%BOMB_ANIMATION_SHAKE_AMPLITUDE) - MAIN_BOMB_ADDITIONAL_SIZE;
        }
        
        animation_state = max(0, animation_state-GEM_ANIMATION_SPEED);
    }
    else {
        if(animation_type == Animation_type::SECONDARY_BOMB_ANIMATE)
            dead = true;
        
        else if(animation_type == Animation_type::MAIN_BOMB_ANIMATE)
            dead = true;
        
        animation_type = Animation_type::IDLE;
    }
    gem.setPosition(sf::Vector2f(position.first, position.second));
    
    window.draw(gem);
}

GameTable::GameTable(const int width, const int height): size({width, height}){
    std::srand(0);
    
    for (int i = 0; i<width;i++){
        gem_table.push_back(vector<Gem*>());
        for (int j = 0; j<height;j++){
            Color color = (Color)(rand()%4);
            gem_table[i].push_back(new Gem(color));
        }
    }
}

void GameTable::paint(sf::RenderWindow& window){
    int table_period = gem_size+gem_gap;
    for (int i = 0; i<gem_table.size();i++){
        for (int j = 0; j<gem_table[i].size();j++){
            gem_table[i][j]->paint({i*table_period + table_position.first, (size.second-1-j)*table_period+table_position.second}, window, gem_size, gem_gap);
            
            if (gem_table[i][j]->dead)
                delete_gem({i,j});
        }
    }

}

void GameTable::paint_black(sf::RenderWindow& window){
    sf::RectangleShape gem({1000.f, 1000.f});
    gem.setFillColor(color2sfml_color(Color::WHITE));
    gem.setPosition(sf::Vector2f(0,0));
    window.draw(gem);
    
}

void GameTable::on_click(pair<int, int> position){
    int column_index = (position.first-table_position.first)/(gem_size+gem_gap);
    int row_index = size.second-1-(position.second-table_position.second)/(gem_size+gem_gap);
    if (prev_click == nullptr){
        prev_click = new pair<int, int>({column_index, row_index});
        
        return;
    }
    swap_gems(*prev_click, {column_index, row_index});
    prev_click = nullptr;
    
    
}

void GameTable::swap_gems(pair<int, int> position1, pair<int, int> position2){
    if (abs(position1.first-position2.first)+abs(position1.second - position2.second)!=1 || gem_table[position1.first][position1.second]->color == Color::WHITE || gem_table[position2.first][position2.second]->color == Color::WHITE)
        return;
    
    if (position1.first > position2.first){
        gem_table[position1.first][position1.second]->animation_type = Animation_type::MOVE_LEFT;
        gem_table[position2.first][position2.second]->animation_type = Animation_type::MOVE_RIGHT;
    }
    
    if (position1.first < position2.first){
        gem_table[position1.first][position1.second]->animation_type = Animation_type::MOVE_RIGHT;
        gem_table[position2.first][position2.second]->animation_type = Animation_type::MOVE_LEFT;
    }
    
    if(position1.second > position2.second){
        gem_table[position1.first][position1.second]->animation_type = Animation_type::MOVE_DOWN;
        gem_table[position2.first][position2.second]->animation_type = Animation_type::MOVE_UP;
    }
    
    if(position1.second < position2.second){
        gem_table[position1.first][position1.second]->animation_type = Animation_type::MOVE_UP;
        gem_table[position2.first][position2.second]->animation_type = Animation_type::MOVE_DOWN;
    }
    
    
    gem_table[position1.first][position1.second]->animation_state = 1;
    gem_table[position2.first][position2.second]->animation_state = 1;
        
    Gem* tmp = gem_table[position1.first][position1.second];
    gem_table[position1.first][position1.second] = gem_table[position2.first][position2.second];
    gem_table[position2.first][position2.second] = tmp;
    
}

void GameTable::delete_gem(pair<int, int> position){
    if (rand()%100 <= BONUS_CHANCE) {
        Bonus* current_bonus = bonus_list[rand()%bonus_list.size()];
    
        vector<pair<int, int>> available_gems;
        for (int i = max(0,position.first-3); i<min(gem_table.size(), position.first+3);i++)
            for (int j = max(0,position.second-3); j<min(gem_table[i].size(), position.second+3);j++)
                if(gem_table[i][j]->color != Color::WHITE && gem_table[i][j]->animation_type == Animation_type::IDLE)
                    available_gems.push_back({i, j});
        
        
        if (available_gems.size() == 0)
            return;
        
        current_bonus->apply(gem_table, available_gems[rand()%available_gems.size()], position);
    }
    
    gem_table[position.first].erase(gem_table[position.first].begin() + position.second);
    gem_table[position.first].push_back(new Gem(Color::WHITE));
    
    for (int i = position.second; i<gem_table[position.first].size(); i++){
        gem_table[position.first][i]->animation_type = Animation_type:: MOVE_DOWN;
        gem_table[position.first][i]->animation_state +=1;
    }
}

void GameTable::check_single_color_sequences(){
    int count;
    for (int i = 0; i<gem_table.size();i++){
        count = 0;
        for (int j = 1; j<gem_table[i].size();j++){
            if (gem_table[i][j]->color != Color::WHITE && gem_table[i][j]->animation_type == Animation_type::IDLE && gem_table[i][j]->color == gem_table[i][j-1]->color)
                count++;
            else{
                if (count >= 2){
                    for (int k = 0; k <=count; k++)
                        delete_gem({i,j-k-1});
                }
                count = 0;
            }
        }
        if (count >= 2){
            for (int k = 0; k <=count; k++)
                delete_gem({i,gem_table[i].size()-1-k});
        }
    }
    
    for (int j = 0; j<gem_table[0].size();j++){
        count = 0;
        for (int i = 1; i<gem_table.size();i++){
            if (gem_table[i][j]->color != Color::WHITE && gem_table[i][j]->animation_type == Animation_type::IDLE && gem_table[i][j]->color == gem_table[i-1][j]->color)
                count++;
            else {
                if (count >= 2){
                    for (int k = 0; k <=count; k++)
                        delete_gem({i-k-1,j});
                }
                count = 0;
            }
        }
        if (count >= 2){
            for (int k = 0; k <=count; k++)
                delete_gem({gem_table.size()-k-1,j});
        }
    }
}


Bonus::Bonus() {}
BombBonus::BombBonus() : Bonus() {}
RepaintBonus::RepaintBonus() : Bonus() {}


void BombBonus::apply(vector<vector<Gem*>>& gem_table, pair<int, int> position, pair<int, int> deleted_gem_position) {
    gem_table[position.first][position.second]->animation_type = Animation_type::MAIN_BOMB_ANIMATE;
    gem_table[position.first][position.second]->animation_state = 1;
    
    vector<pair<int, int>> available_gems;
    
    for (int i = 0; i<gem_table.size();i++)
        for (int j = 0; j<gem_table[i].size();j++)
            if(gem_table[i][j]->color != Color::WHITE && gem_table[i][j]->animation_type == Animation_type::IDLE && (i != position.first || j != position.second))
                available_gems.push_back({i, j});
    
    for (int i = 0; i<5;i++){
        if (available_gems.size() == 0)
            return;
        
        int index = rand()%available_gems.size();
        
        gem_table[available_gems[index].first][available_gems[index].second]->animation_type = Animation_type::SECONDARY_BOMB_ANIMATE;
        gem_table[available_gems[index].first][available_gems[index].second]->animation_state = 1;
        
        
        available_gems.erase(available_gems.begin() + index);
    }
}

void RepaintBonus::apply(vector<vector<Gem*>>& gem_table, pair<int, int> position, pair<int, int> deleted_gem_position) {
    Color color = gem_table[deleted_gem_position.first][deleted_gem_position.second]->color;
    
    gem_table[position.first][position.second]->color = color;
    
    vector<pair<int, int>> available_gems;
    
    for (int i = 0; i<gem_table.size();i++)
        for (int j = 0; j<gem_table[i].size();j++)
            if(gem_table[i][j]->color != Color::WHITE && gem_table[i][j]->animation_type == Animation_type::IDLE && (i != position.first || j != position.second) && abs(position.first-i)+abs(position.second - j)>1)
                available_gems.push_back({i, j});
    
    for (int i = 0; i<3;i++){
        if (available_gems.size() == 0)
            return;
        
        int index = rand()%available_gems.size();
        
        gem_table[available_gems[index].first][available_gems[index].second]->color = color;
        
        
        
        available_gems.erase(available_gems.begin() + index);
    }

}
