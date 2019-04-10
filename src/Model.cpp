#include "../include/Model.h"
#include <sstream>
#include <math.h>
using namespace std;
#define NO_TRG_CIRCLE 10

Model::Model(){
    translate = glm::mat4(1.0f);
    rotate = glm::mat4(1.0f);
    scale = 1.0;
    is_selected = false;
    is_light_source = false;
    is_select_rotate = false;
}

Model::~Model(){
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

string Model::get_file_format() const{
    return file_format;
}

string Model::get_data_format() const{
    return data_format;
}

vector<string> Model::get_comments() const{
    return comments;
}

GLfloat Model::get_version_no() const{
    return version_no;
}

int Model::get_no_vertices() const{
    return no_vertices;
}

int Model::get_no_faces() const{
    return no_faces;
}

glm::mat4 Model::get_translate() const{
    return translate;
}

GLfloat Model::get_scale() const{
    return scale;
}

glm::mat4 Model::get_rotate() const{
    return rotate;
}

vector<GLfloat> Model::get_vertices() const {
    return vertices;
}

vector<unsigned int> Model::get_indices() const {
    return indices;
}

vector<Color> Model::get_vertices_color() const{
    return vertices_color;
}

glm::vec3 Model::get_cursor_pos() const {
    return currpos;
}

glm::mat4 Model::get_model() const{
    glm::mat4 scale_matrix = glm::mat4(1.0f);
    scale_matrix = glm::scale(scale_matrix,glm::vec3(scale,scale,scale));
    return translate*rotate*scale_matrix;
}

unsigned int Model::get_VAO() const{
    return VAO;
}

const Texture & Model::get_texture() const{
    return texture;
}

bool Model::get_light_source() const {
    return (is_light_source == true);
}

bool Model::is_select() const{
    return is_selected;
}

bool Model::get_is_select_rotate() const{
    return (is_select_rotate == true);
}

Color Model::get_vertex_color(int idx) const{
    return vertices_color[idx];
}

void Model::set_vertex_color(int idx,const Color& color){
    vertices_color[idx] = color;
}

void Model::set_cursor_pos(const glm::vec3& pos){
    currpos = pos;
}

void Model::set_translate(const glm::mat4& tr){
    translate = tr;
}

void Model::set_light_source(bool val) {
    is_light_source = val;
}

void Model::set_scale(GLfloat sc) {
    scale = sc;
}

void Model::set_rotate(const glm::mat4& rr) {
    rotate = rr;
}
void Model::set_selected(bool val) {
    is_selected = val;
}

void Model::set_select_rotate(bool val) {
    is_select_rotate = val;
}

void Model::set_texture(const Texture& tex) {
    texture = tex;
    compute_texture_mapping();
}

void Model::pass_info_lightshader(){
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);
    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices()[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*get_indices().size(),
                                            &get_indices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
}

void Model::pass_info_shader(){
    if(is_light_source){
        pass_info_lightshader();
        return;
    }
    glGenVertexArrays(1,&VAO);  //here object is created but memory is not provided.
    glGenBuffers(1,&VBO);       //same here.
    glGenBuffers(1,&EBO);
    //binds object to context.
    glBindVertexArray(VAO);     
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices()[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*get_indices().size(),
                                            &get_indices()[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(0);
    unsigned int ColorBuffer;
    glGenBuffers(1,&ColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,ColorBuffer);
    try{
        if(get_vertices().size() != 3*get_vertices_color().size()){
            throw string("Size of vertices and vertices_color is not same");
        }
    }
    catch(const string& s){
        cout << s << endl;
    }
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*get_vertices().size(),&get_vertices_color()[0],GL_STATIC_DRAW);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(void*)0);
    glEnableVertexAttribArray(1);

    unsigned int textureBuffer;
    glGenBuffers(1,&textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,textureBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*texture.get_plane().size(),&texture.get_plane()[0],GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

}

bool is_between(float mini,float maxi,float value){
    return (value>=mini && value<=maxi);
}

bool Model::is_inside(Point trans_coord){
    // cout << "mini:" << mini << endl;
    // cout << "maxi:" << maxi << endl;
    if(is_between(mini.getX(),maxi.getX(),trans_coord.getX()) &&
      is_between(mini.getY(),maxi.getY(),trans_coord.getY()) && 
      is_between(mini.getZ(),maxi.getZ(),trans_coord.getZ())){
        return true;
    }
    else{
        return false;
    }
}

void Model::set_mini(const Point& point){
    mini = point;
}

void Model::set_maxi(const Point& point){
    maxi = point;
}

void Model::compute_adj_list(){
    int no_trg = indices.size()/3;
    adj_list.resize(vertices.size()/3);
    for(int i = 0; i < no_trg; i++){
        adj_list[indices[i*3]].push_back(i);
        adj_list[indices[i*3+1]].push_back(i);
        adj_list[indices[i*3+2]].push_back(i);
    }
}

vector<Point> Model::compute_normals() {
    int no_trg = indices.size()/3;
    vector<Point> trg_normal;
    for(int i = 0; i < no_trg; i++){
        Point p1 = Point(vertices[3*indices[i*3+0]],vertices[3*indices[i*3+0]+1],vertices[3*indices[i*3+0]+2]);
        Point p2 = Point(vertices[3*indices[i*3+1]],vertices[3*indices[i*3+1]+1],vertices[3*indices[i*3+1]+2]);
        Point p3 = Point(vertices[3*indices[i*3+2]],vertices[3*indices[i*3+2]+1],vertices[3*indices[i*3+2]+2]);
        // cout << i << p1 << p2 << p3;
        Point V = p2-p1;
        Point W = p3-p1;
        Point normal;
        normal.setX(V.getY()*W.getZ() - V.getZ()*W.getY());
        normal.setY(V.getZ()*W.getX() - V.getX()*W.getZ());
        normal.setZ(V.getX()*W.getY() - V.getY()*W.getX());
        float mag = sqrt(normal.getX()*normal.getX() + normal.getY()*normal.getY() + normal.getZ()*normal.getZ());
        normal.normalize(mag); 
        // cout << i << " "<< normal;
        trg_normal.push_back(normal);
    }
    return trg_normal;
}

void Model::compute_vertices_color(){
    vector<Point> trg_normal = compute_normals();
    compute_adj_list();
    for(int i = 0; i < adj_list.size(); i++){
        Point p(0,0,0);
        for(auto e:adj_list[i]){
            p = p + trg_normal[e];
        }
        p.normalize(adj_list[i].size());
        float mag = sqrt( p.getX()*p.getX() + p.getY()*p.getY() + p.getZ()*p.getZ() );
        p.normalize(mag);
        vertices_color.push_back(Color(abs(p.getX()),abs(p.getY()),abs(p.getZ())));
    }   
}

void Model::compute_spherical() {
    glm::vec3 center;
    center.x = (maxi.getX() + mini.getX()) * 0.5f;
    center.y = (maxi.getY() + mini.getY()) * 0.5f;
    center.z = (maxi.getZ() + mini.getZ()) * 0.5f;
    float r = (maxi.getX() - mini.getX()) * 0.5f;
    vector<float> spherical;
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec3 p;
        p.x = vertices[i*3];
        p.y = vertices[i*3+1];
        p.z = vertices[i*3+2];
        glm::vec2 tex;
        p.x = p.x - center.x;
        p.y = p.y - center.y;
        p.z = p.z - center.z;
        tex.x = atan2(-p.z, p.x) / (2 * 3.14);
        tex.x+=3.14;
        tex.y = (acos(-p.y / r) / 3.14);
        spherical.push_back(tex.x);
        spherical.push_back(tex.y);
    }
    texture.set_spherical(spherical);
}

void Model::compute_cylindrical() {
    float height = maxi.getZ() - mini.getZ();
    float r = (maxi.getX() - mini.getX()) * 0.5f;
    vector<float> cylindrical;
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec2 tex;
        glm::vec3 pos;
        pos.x = vertices[i*3];
        pos.y = vertices[i*3+1];
        pos.z = vertices[i*3+2];
        tex.x = acosf(pos.x / r) / (2 * 3.14f);
        tex.y = pos.z;
        cylindrical.push_back(tex.x);
        cylindrical.push_back(tex.y);
    }
    texture.set_cylindrical(cylindrical);
}

void Model::compute_plane() {
    vector<float> plane;
    for (int i = 0; i < (vertices.size()/3) ; i++) {
        glm::vec2 tex;
        glm::vec3 pos;
        pos.x = vertices[i*3];
        pos.y = vertices[i*3+1];
        pos.z = vertices[i*3+2];
        tex.x = (pos.x - mini.getX()) / (maxi.getX() - mini.getX());
        tex.y = (pos.y - mini.getY()) / (maxi.getY() - mini.getY());
        plane.push_back(tex.x);
        plane.push_back(tex.y);
    }
    texture.set_plane(plane);
}

void Model::compute_texture_mapping() {
    compute_spherical();
    compute_cylindrical();
    compute_plane();
}

Point Model::transform(const Point& point,const Point& normal,const Point& incentre,float inradii){
    //do rotation first then translate and then scale.
    glm::vec3 circle_normal = glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 trg_normal = glm::vec3(normal.getX(),normal.getY(),normal.getZ());
    trg_normal = glm::normalize(trg_normal);
    glm::vec3 axis = glm::cross(trg_normal,circle_normal);
    axis = glm::normalize(axis);
    float angle = acos(glm::dot(circle_normal,trg_normal));

    glm::vec3 inctr = glm::vec3(incentre.getX(),incentre.getY(),incentre.getZ()); 
    glm::mat4 rotate = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);

    if(angle != 0)
        rotate = glm::rotate(rotate,-angle,axis);
    translate = glm::translate(translate,inctr);
    scale = glm::scale(scale,glm::vec3(inradii,inradii,inradii));

    glm::vec4 pt = glm::vec4(point.getX(),point.getY(),point.getZ(),1.0f);
    glm::mat4 main_matrix = translate*rotate*scale;
    glm::vec4 newpt = main_matrix*pt;

    return Point(newpt.x,newpt.y,newpt.z);
}

std::ifstream& safeGetline(std::ifstream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::ifstream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

//assumptions:
//vertices will be in float format only.
//no of dimensions = 3.
ifstream & operator >> (ifstream &fin, Model &model){
    string temp;
    fin >> temp >> model.file_format >> model.data_format;
    float minx=1e5,miny=1e5,minz=1e5;
    float maxx=-1e5,maxy=-1e5,maxz=-1e5;
    while(1){
        fin >> temp;
        if(temp != "comment") break;
        string comment;
        safeGetline(fin,comment);
        model.comments.push_back(comment);
    }
    fin >> temp >> model.no_vertices;
    while(1){
        fin >> temp;
        if(temp != "property") break;
        fin >> temp >> temp;
        // do nothing.
    }
    fin >> temp >> model.no_faces;
    while(1){
        safeGetline(fin,temp);
        if(temp == "end_header"){
            break;
        }
    }
    float maxi=0;
    for(int i = 0; i < model.no_vertices; i++){
        float temp1;
        float arr[3];
        //change to absolute value
        for(int j = 0; j < 3; j++){
            fin >> temp1;
            maxi=max(maxi,abs(temp1));
            model.vertices.push_back(temp1);
            if(j==0){
                minx = min(minx,temp1);
                maxx = max(maxx,temp1);
            }
            else if(j==1){
                miny = min(miny,temp1);
                maxy = max(maxy,temp1);
            }
            else{
                minz = min(minz,temp1);
                maxz = max(maxz,temp1);
            }
        }
    }
    float maxdim = max(maxx - minx, max(maxy - miny, maxz - minz));  
    for(int i=0;i<model.vertices.size()/3;i++){
        model.vertices[i*3] = (model.vertices[i*3] - minx )/ maxdim;
        model.vertices[i*3+1] = (model.vertices[i*3+1] - miny )/ maxdim;
        model.vertices[i*3+2] = (model.vertices[i*3+2] - minz )/ maxdim;
        // cout << "dart::" << model.vertices[i*3] << " " << model.vertices[i*3+1] << " "
        //  << model.vertices[i*3+2] << " " << endl;
    }
    for(int i = 0; i < model.no_faces; i++){
        unsigned int tmp;
        fin >> tmp;
        unsigned int tmp1;
        for(int j = 0; j < tmp; j++){
            fin >> tmp1;
            model.indices.push_back(tmp1);
        }
    }

    float minX = 2.0,minY=2.0,minZ=2.0;
    float maxX=-2.0,maxY=-2.0,maxZ=-2.0;
    for(int i=0;i<model.vertices.size()/3;i++){
        minX = min(minX,model.vertices[i*3]);
        maxX = max(maxX,model.vertices[i*3]);
        minY = min(minY,model.vertices[i*3+1]);
        maxY = max(maxY,model.vertices[i*3+1]);
        minZ = min(minZ,model.vertices[i*3+2]);
        maxZ = max(maxZ,model.vertices[i*3+2]);
    }

    model.set_mini(Point(minX,minY,minZ));
    model.set_maxi(Point(maxX,maxY,maxZ));

    //compute color of the vertices.
    model.compute_vertices_color();
    return fin;
}
