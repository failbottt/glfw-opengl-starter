const char *texture_vert_shader = "shaders/texture.vert";
const char *texture_frag_shader = "shaders/texture.frag";
const char *quad_vert_shader_path = "shaders/quad.vert";
const char *quad_frag_shader_path = "shaders/quad.frag";

GLuint vertex_shader;
GLuint frag_shader;
GLuint quad_vertex_shader;
GLuint quad_frag_shader;
GLuint texture_program;
GLuint quad_program;

	void 
init_gl_programs() 
{
	const char *vert_shader_source = read_file(texture_vert_shader);
	const char *frag_shader_source = read_file(texture_frag_shader);
	const char *font_vert_shader_source = read_file(quad_vert_shader_path);
	const char *quad_frag_shader_source = read_file(quad_frag_shader_path);

	// compiles vert shader
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vert_shader_source, NULL);
	glCompileShader(vertex_shader);
	checkCompileErrors(vertex_shader, "VERTEX");

	quad_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(quad_vertex_shader, 1, &font_vert_shader_source, NULL);
	glCompileShader(quad_vertex_shader);
	checkCompileErrors(quad_vertex_shader, "QUADVERT");

	// compiles frag shader
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &frag_shader_source, NULL);
	glCompileShader(frag_shader);
	checkCompileErrors(frag_shader, "FRAG");

	quad_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(quad_frag_shader, 1, &quad_frag_shader_source, NULL);
	glCompileShader(quad_frag_shader);
	checkCompileErrors(quad_frag_shader, "QUADFRAG");

	// create program
	texture_program = glCreateProgram();
	glAttachShader(texture_program, vertex_shader);
	glAttachShader(texture_program, frag_shader);
	glLinkProgram(texture_program);

	quad_program = glCreateProgram();
	glAttachShader(quad_program, quad_vertex_shader);
	glAttachShader(quad_program, quad_frag_shader);
	glLinkProgram(quad_program);

	// delete shaders because they are part of the program now
	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);
	glDeleteShader(quad_vertex_shader);
	glDeleteShader(quad_frag_shader);
}

void gl_set_perspective_as_ortho(U64 screen_width, U64 screen_height) 
{
	F32 pj[4][4] = {{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},  
		{0.0f, 0.0f, 0.0f, 0.0f},                    \
		{0.0f, 0.0f, 0.0f, 0.0f}};

	F32 rl, tb, fn;
	F32 left = 0.0f;
	F32 right = (F32)screen_width;
	F32 bottom = 0.0f;
	F32 top = (F32)screen_height;
	F32 nearZ = -1.0f;
	F32 farZ = 1.0f;

	rl = 1.0f / (right  - left);
	tb = 1.0f / (top    - bottom);
	fn =-1.0f / (farZ - nearZ);

	pj[0][0] = 2.0f * rl;
	pj[1][1] = 2.0f * tb;
	pj[2][2] = 2.0f * fn;
	pj[3][0] =-(right  + left)    * rl;
	pj[3][1] =-(top    + bottom)  * tb;
	pj[3][2] = (farZ + nearZ) * fn;
	pj[3][3] = 1.0f;
	// ORTHO END

	glUseProgram(texture_program);
	glUniformMatrix4fv(glGetUniformLocation(texture_program, "projection"), 1, GL_FALSE, (F32 *)pj);

	glUseProgram(quad_program);
	glUniformMatrix4fv(glGetUniformLocation(quad_program, "projection"), 1, GL_FALSE, (F32 *)pj);
}
