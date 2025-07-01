attribute vec3 a_posL; 
attribute vec3 a_norm;
uniform mat4 uMVP;   

varying vec3 v_coord;  

void main() {
   
    gl_Position = uMVP * vec4(a_posL, 1.0);
    
    v_coord = a_posL;
}