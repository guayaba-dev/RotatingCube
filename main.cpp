#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <dos.h>


struct vec3 
{
    int x,y,z{0};
    vec3(){}
    vec3(int i, int j, int k){this->x = i; this->y = j; this->z = k;}
};

struct vec2
{
    int x,y{0};
    vec2(){}
    vec2(int i, int j){this->x = i; this->y = j;}    

    bool operator<(const vec2& other) const {
        // Comparación basada en los componentes x e y del vector
        if (x != other.x) {
            return x < other.x;
        } else {
            return y < other.y;
        }
    }
    
    bool operator>(const vec2& other) const {
        // Comparación basada en los componentes x e y del vector    
        if (x != other.x) {                              
            return x > other.x;                        
        } else {                                   
            return y < other.y;                  

        }                                                                
    }

    bool operator==(const vec2& other) const {
        // Comparación basada en los componentes x e y del vector
        return (x == other.x) && (y == other.y);
    }

};

int projectionCaculation(float fov, float a, float z){
    float numerator = a*fov;
    float denominator = fov + z;
    float projectionR = numerator/denominator;
    int sing = (projectionR > 0) ? 1 : -1;
    int nProjectionR = ((std::abs(projectionR) - std::abs(int(projectionR))) >= 0.5) ? std::ceil(std::abs(projectionR)) : int(std::abs(projectionR));
    return nProjectionR*sing;
};


std::vector<vec2> verticeProjection(std::vector<vec3> vertices){
    int fov{63};
    std::vector<vec2> projectionVertices{};

    for(auto i : vertices){
        int projectionX,projectionY{};         
        projectionX = projectionCaculation(fov, i.x, i.z) + 30;
        projectionY = projectionCaculation(fov, i.y, i.z) + 15;
        projectionVertices.push_back(vec2(projectionX, projectionY));
    }
    
    return projectionVertices;

};

void getLinePoints(vec2 v1, vec2 v2, std::map<vec2, int> *renderingPoints) {
    int x = v1.x;
    int y = v1.y;
    int deltaX = std::abs(v2.x - v1.x);
    int deltaY = std::abs(v2.y - v1.y);
    int s1 = (v2.x > v1.x) ? 1 : -1;
    int s2 = (v2.y > v1.y) ? 1 : -1;

    int interchange = 0;

    if (deltaY > deltaX) {
        std::swap(deltaX, deltaY);
        interchange = 1;
    }

    int e = 2 * deltaY - deltaX;
    int a = 2 * deltaY;
    int b = 2 * deltaY - 2 * deltaX;

    (*renderingPoints)[vec2(x,y)] = 1;


    for (int i = 1; i <= deltaX; ++i) {
        if (e < 0) {
            if (interchange == 1) {
                y = y + s2;
            } else {
                x = x + s1;
            }
            e = e + a;
        } else {
            y = y + s2;
            x = x + s1;
            e = e + b;
        }

    (*renderingPoints)[vec2(x,y)] = 1;

    }

}

void drawWireFrame(std::vector<vec2> vertices, std::vector<std::vector<int>> edges, std::map<vec2, int> *rederingPoints){
    for(auto i : edges){

        vec2 v1{vertices[i[0]]};
        vec2 v2{vertices[i[1]]};

        getLinePoints(v1, v2, rederingPoints);
    }
   
};

void drawpixel(vec2 size, std::map<vec2,int> renderingPoints){

    system("cls");

    for(int j{1}; j <= size.y; j++){
        
        for(int i{1}; i <= size.x; i++){
            
            if(renderingPoints.count(vec2(i,j)) > 0){

               std::cout << "#";


            }
            else{
                std::cout << " "; 
            }

        }

        std::cout << std::endl;

    }


}

bool matrixRotation(double t, std::vector<vec3> *vertices){

    for(auto& vector : *vertices){

       vector.x = vector.y*std::sin(t)*std::sin(t)*std::cos(t) - vector.z*std::cos(t)*std::sin(t)*std::cos(t) + vector.y*std::cos(t)*std::sin(t) + vector.z*std::sin(t)*std::sin(t) + vector.x*std::cos(t)*std::cos(t);
       vector.y = vector.y*std::cos(t)*std::cos(t) + vector.z*std::sin(t)*std::cos(t) - vector.y*std::sin(t)*std::sin(t)*std::sin(t) + vector.z*std::cos(t)*std::sin(t)*std::sin(t) - vector.x*std::cos(t)*std::sin(t);
       vector.z = vector.z*std::cos(t)*std::cos(t) - vector.y*std::sin(t)*std::cos(t) + vector.x*std::sin(t);

    }
    
    return true;
}

int main(){

// CUBE Wireframe

std::vector<std::vector<int>> lines{{0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},{0,4},{1,5},{2,6},{3,7}};

float t{0.2};
std::map<vec2,int> nrenderingPoints{};
std::map<vec2,int> crenderingPoints{};
while (1)
{     
    std::vector<vec3> vertices{vec3(-8,8,1), vec3(8,8,1), vec3(8,-8,1), vec3(-8,-8,1),
                               vec3(-8,8,16), vec3(8,8,16), vec3(8,-8,16), vec3(-8,-8,16)};

    if(!nrenderingPoints.empty()){ 
    drawpixel(vec2(60,30), crenderingPoints);
    nrenderingPoints.clear();
    }
    else{
    matrixRotation(t, &vertices);
    drawWireFrame(verticeProjection(vertices), lines, &nrenderingPoints);
    t += 0.05;
    std::swap(nrenderingPoints, crenderingPoints); 
    }

}

return 0;
}