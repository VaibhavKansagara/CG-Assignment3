#ifndef Model_H_
#define Model_H_

#include <bits/stdc++.h>

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

extern unsigned int no_t_press;
extern unsigned int no_m_press;

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Point.h"
#include "Color.h"
#include "Texture.h"

using namespace std;

#define PI 3.14159

class Model{
public:
    Model();

    ~Model();

    string get_file_format() const;

    string get_data_format() const;

    vector<string> get_comments() const;

    float get_version_no() const;

    int get_no_vertices() const;

    int get_no_faces() const;

    Color get_vertex_color(int idx) const; 

    glm::vec3 get_cursor_pos() const;
    
    vector<GLfloat> get_vertices() const ;

    vector<Color> get_vertices_color() const;

    vector<unsigned int> get_indices() const;

    glm::mat4 get_translate() const;

    glm::mat4 get_rotate() const;

    glm::vec3 get_lightpos() const;

    GLfloat get_scale() const;

    glm::mat4 get_model() const;

    bool get_light_source() const;

    unsigned int get_VAO() const;

    const Texture& get_texture() const;

    bool is_select() const;

    bool get_is_select_rotate() const;

    vector<float> get_spherical() const;

    vector<float> get_cylindrical() const;

    vector<float> get_plane() const;

    void set_vertex_color(int idx,const Color& color);

    void set_translate(const glm::mat4& tr);

    void set_scale(GLfloat sc);

    void set_light_no(int no);

    void set_rotate(const glm::mat4& rr);

    void set_cursor_pos(const glm::vec3& pos);

    void set_selected(bool val);

    void set_mini(const Point& point);

    void set_maxi(const Point& point);

    void set_light_source(bool val);

    void set_select_rotate(bool val);

    void set_lightpos(glm::vec3 pos);

    void set_texture(const Texture& tex);

    void set_spherical(const vector<float>& sph);

    void set_cylindrical(const vector<float>& cyl);

    void set_plane(const vector<float>& pl);

    bool is_inside(Point trans_coord);

    void compute_adj_list();

    void compute_spherical();

    void compute_cylindrical();

    void compute_plane();

    void compute_texture_mapping();

    void compute_vertices_color();

    vector<Point> compute_normals();

    void pass_info_shader();

    void pass_info_lightshader();

    void change_mapping();

    Point transform(const Point& point,const Point& normal,const Point& incentre,float inradii);

    friend ifstream & operator >> (ifstream &fin, Model &model);
private:
    string file_format,data_format;
    GLfloat version_no;
    vector<string> comments;
    int no_vertices,no_faces;
    bool header;
    vector<GLfloat> vertices;
    vector<unsigned int> indices;
    vector< vector<int> > adj_list;
    vector<Color> vertices_color;
    glm::vec3 currpos;
    GLfloat scale;
    glm::mat4 translate,rotate;
    Point mini,maxi;
    bool is_selected,is_light_source,is_select_rotate;
    unsigned int VAO,VBO,EBO,textureVBO;
    Texture texture;
    vector<float> spherical,cylindrical,plane;
    int light_no;
    glm::vec3 lightpos;
};

#endif