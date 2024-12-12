#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

typedef std::array<int, 2> pos;
enum {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};
typedef std::array<int, 3> pos_side;
typedef std::array<bool, 4> box_edges;

void merge_and_count_edges(std::vector<std::vector<box_edges>> &grid, int x, int y, int dir)
{
    if (!(0 <= x && x < grid.size()) || !(0 <= y && y < grid[x].size())) {
        return;
    }
    if (!grid[x][y][dir]) {
        return;
    }
    
    grid[x][y][dir] = false;
    if (dir == TOP || dir == BOTTOM) {
        merge_and_count_edges(grid, x, y - 1, dir);
        merge_and_count_edges(grid, x, y + 1, dir);
    } else {
        merge_and_count_edges(grid, x - 1, y, dir);
        merge_and_count_edges(grid, x + 1, y, dir);
    }
}

std::array<int, 2> floodfill(std::vector<std::string>& grid, int x, int y, char type, std::vector<pos_side> &edges, int dir)
{
    if (!(0 <= x && x < grid.size()) || !(0 <= y && y < grid[x].size())) {
        edges.push_back({x+1, y+1, dir});
        return {0, 1};
    }
    if (grid[x][y] == type - 64) 
        return {0, 0};
    if (grid[x][y] != type) {
        edges.push_back({x+1, y+1, dir});
        return {0, 1}; //special return for borders
    }
    grid[x][y] = type - 64;
    std::array<int, 2> out = {1, 0}, temp;
    temp = floodfill(grid, x-1, y, type, edges, BOTTOM);
    out[0] += temp[0]; out[1] += temp[1];
    temp = floodfill(grid, x+1, y, type, edges, TOP);
    out[0] += temp[0]; out[1] += temp[1];
    temp = floodfill(grid, x, y-1, type, edges, RIGHT);
    out[0] += temp[0]; out[1] += temp[1];
    temp = floodfill(grid, x, y+1, type, edges, LEFT);
    out[0] += temp[0]; out[1] += temp[1];
    return out;
}

int main()
{
    std::ifstream in("input.txt");
    std::string line;
    std::vector<std::string> grid;
    while (getline(in, line))
        grid.push_back(line);

    long long p2 = 0;
    for (int i=0; i<grid.size(); i++) {
        for (int j=0; j<grid[i].size(); j++) {
            if (grid[i][j] >= 64) {
//                std::cout << grid[i][j] << '\n';
                std::vector<pos_side> edges;
                std::array<int, 2> temp = floodfill(grid, i, j, grid[i][j], edges, 0);
                std::vector<std::vector<box_edges>> edgegrid;
                for (int n=0; n<grid.size() + 2; n++) {
                    edgegrid.push_back(std::vector<box_edges>(grid[0].size()+2)); //layer of buffer
                }
                for (pos_side square : edges) {
                    edgegrid[square[0]][square[1]][square[2]] = true;
                }
                int out=0;
                bool any_left = true;
                while (any_left) {
                    any_left = false;
                    for (int x=0; x<edgegrid.size(); x++) {
                        for (int y=0; y<edgegrid[x].size(); y++) {
                            if (edgegrid[x][y][TOP]) {
                                out++;
                                merge_and_count_edges(edgegrid, x, y, TOP);
                                any_left = true;
                            } else if (edgegrid[x][y][BOTTOM]) {
                                out++;
                                merge_and_count_edges(edgegrid, x, y, BOTTOM);
                                any_left = true;
                            } else if (edgegrid[x][y][LEFT]) {
                                out++;
                                merge_and_count_edges(edgegrid, x, y, LEFT);
                                any_left = true;
                            } else if (edgegrid[x][y][RIGHT]) {
                                out++;
                                merge_and_count_edges(edgegrid, x, y, RIGHT);
                                any_left = true;
                            }
                        }
                    }
                }
//                std::cout << "temp[0]: " << temp[0] << ", out: " << out << '\n';
                p2 += temp[0] * out;
            }
        }
    }
    std::cout << "p2: " << p2 << '\n';
}