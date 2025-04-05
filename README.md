# The Boys - Superhero Simulation (2024/2)

![GitHub last commit](https://img.shields.io/github/last-commit/pietrocom/theboys)
![GitHub repo size](https://img.shields.io/github/repo-size/pietrocom/theboys)

A discrete event simulation of a superhero world with heroes, bases, and missions - originally conceived by Profs. Fabiano Silva, Luis Bona, and Marcos Castilho for CI1001/CI067 courses at UFPR.

## Project Description

This simulation models a dynamic world where:
- **Heroes** with unique abilities (flight, invisibility, x-ray vision, etc.) travel between bases
- **Bases** manage hero arrivals/departures with limited capacity
- **Missions** require specific skill combinations from hero teams
- Events unfold over 1 simulated year (525,600 minutes)

Key features:
- Heroes gain experience from successful missions
- Mortality risk during dangerous missions
- Queue management at bases
- Distance-based travel calculations
- Comprehensive statistics tracking

## Technical Specifications

### Core Components
- **Discrete Event Simulation** engine
- **Priority Queue** for event scheduling
- **Hero/Base/Mission** entity management
- **Skill matching** algorithms

### Data Structures
```c
struct Hero {
    int id;
    Set* abilities;
    int patience;
    int speed;
    int experience;
    int current_base;
    bool alive;
};

struct Base {
    int id;
    int capacity;
    Set* present_heroes;
    List* waiting_queue;
    Point location;
};

struct Mission {
    int id;
    Set* required_abilities;
    int danger_level;
    Point location;
    bool completed;
};
