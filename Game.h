//
//  Game.h
//  SFML_TTT
//
//  Created by Alex on 19.05.2025.
//
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

enum class Color{
    GREEN, YELLOW, PURPLE, PINK, WHITE, RED
};

enum class Animation_type{
    MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN, IDLE, MAIN_BOMB_ANIMATE, SECONDARY_BOMB_ANIMATE
};

class Gem{
public:
    Animation_type animation_type = Animation_type::IDLE;
    double animation_state = 0;
    
    bool dead = false;
    
    Color color;
    
    
    Gem(Color color) : color(color) {};
    
    void paint(pair<int, int> position, sf::RenderWindow& window, int gem_size, int gem_gap);
};

class Bonus{
public:
    virtual void apply(vector<vector<Gem*>>& gem_table, pair<int, int> position,  pair<int, int> deleted_gem_position) = 0;
    Bonus();
};

class BombBonus: public Bonus {
public:
    virtual void apply(vector<vector<Gem*>>& gem_table, pair<int, int> position, pair<int, int> deleted_gem_position);
    
    BombBonus();
};

class RepaintBonus: public Bonus {
public:
    virtual void apply(vector<vector<Gem*>>& gem_table, pair<int, int> position, pair<int, int> deleted_gem_position);
    
    RepaintBonus();
};

    
class GameTable{
private:
    vector<vector<Gem*>> gem_table;
    pair<int,int>* prev_click;
    int gem_size = 50;
    int gem_gap = 5;
    pair<int, int> table_position {100,100};
    pair<int, int> size;
    vector<Bonus*> bonus_list {new BombBonus(), new RepaintBonus()};
    
public:
    Color color;
    void paint(sf::RenderWindow& window);
    void paint_black(sf::RenderWindow& window);
    void on_click (pair<int, int> position);
    void swap_gems(pair<int, int> position1, pair<int, int> position2);
    void check_single_color_sequences();
    void delete_gem(pair<int, int> position);
    GameTable(const int width, const int height);
};
