//
// Created by F1 on 11/21/2016.
//

#include "Window_Model_Holder.hpp"

Window_Model_Holder::Window_Model_Holder(const char* filepath)
{
	tile_model = new Static_Model(filepath);
	if(!tile_model)
	{
		LOGE("Window Model Holder Error: model failed to load, aborting mesh matrix calculation");
		return;
	}

	Mat4 ident = Mat4::IDENTITY();

	//================== Making the matrices =================

	//Making a 1x2 strip
	m1x2_model = new Static_Model(tile_model,ident,tile_model,Mat4::TRANSLATE(Vec3(TILE_SIZE,0,0)));
	//Making 2x2 model
	m2x2_model = new Static_Model(m1x2_model,ident,m1x2_model,Mat4::TRANSLATE(Vec3(0,0,WINDOW_TILE_SIZE)));

	//Making a temp 2x4 strip
	Static_Model temp_2x4 = Static_Model(m2x2_model,ident,m2x2_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*2,0,0)));;
	//Making 4x4 model
	m4x4_model = new Static_Model(&temp_2x4,ident,&temp_2x4,Mat4::TRANSLATE(Vec3(0,0,2*WINDOW_TILE_SIZE)));

	//Making a temp 4x8 strip
	Static_Model temp_4x8 = Static_Model(m4x4_model,ident,m4x4_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*4,0,0)));;
	//Making 8x8 model
	m8x8_model = new Static_Model(&temp_4x8,ident,&temp_4x8,Mat4::TRANSLATE(Vec3(0,0,4*WINDOW_TILE_SIZE)));

	//Making a temp 8x16 strip
	Static_Model temp_8x16 = Static_Model(m8x8_model,ident,m8x8_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*8,0,0)));;
	//Making 16x16 model
	m16x16_model = new Static_Model(&temp_8x16,ident,&temp_8x16,Mat4::TRANSLATE(Vec3(0,0,8*WINDOW_TILE_SIZE)));

	//Making a temp 16x32 strip
	Static_Model temp_16x32 = Static_Model(m16x16_model,ident,m16x16_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*16,0,0)));;
	//Making 16x16 model
	m32x32_model = new Static_Model(&temp_16x32,ident,&temp_16x32,Mat4::TRANSLATE(Vec3(0,0,16*WINDOW_TILE_SIZE)));


	//=================== Making the horizontal strips ==================
	//1x2 strip is already made above.
	m1x4_model = new Static_Model(m1x2_model,ident,m1x2_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*2,0,0)));
	m1x8_model = new Static_Model(m1x4_model,ident,m1x4_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*4,0,0)));
	m1x16_model = new Static_Model(m1x8_model,ident,m1x8_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*8,0,0)));
	m1x32_model = new Static_Model(m1x16_model,ident,m1x16_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*16,0,0)));


	//=================== Making the vertical strips ==================
	m2x1_model = new Static_Model(tile_model,ident,tile_model,Mat4::TRANSLATE(Vec3(0,0,WINDOW_TILE_SIZE)));
	m4x1_model = new Static_Model(m2x1_model,ident,m2x1_model,Mat4::TRANSLATE(Vec3(0,0,2*WINDOW_TILE_SIZE)));
	m8x1_model = new Static_Model(m4x1_model,ident,m4x1_model,Mat4::TRANSLATE(Vec3(0,0,4*WINDOW_TILE_SIZE)));
	m16x1_model = new Static_Model(m8x1_model,ident,m8x1_model,Mat4::TRANSLATE(Vec3(0,0,8*WINDOW_TILE_SIZE)));
	m32x1_model = new Static_Model(m16x1_model,ident,m16x1_model,Mat4::TRANSLATE(Vec3(0,0,16*WINDOW_TILE_SIZE)));
}

Window_Model_Holder::~Window_Model_Holder ()
{
	delete tile_model;
	delete m2x2_model;
	delete m4x4_model;
	delete m8x8_model;
	delete m16x16_model;
	delete m32x32_model;

	//Horizontal strips
	delete m1x2_model;
	delete m1x4_model;
	delete m1x8_model;
	delete m1x16_model;
	delete m1x32_model;

	//Vertical strips
	delete m2x1_model;
	delete m4x1_model;
	delete m8x1_model;
	delete m16x1_model;
	delete m32x1_model;
}

int Window_Model_Holder::init_gl()
{
	if(tile_model)
		tile_model->init_gl();
	if(m2x2_model)
		m2x2_model->init_gl();
	if(m4x4_model)
		m4x4_model->init_gl();
	if(m8x8_model)
		m8x8_model->init_gl();
	if(m16x16_model)
		m16x16_model->init_gl();
	if(m32x32_model)
		m32x32_model->init_gl();

	//Horizontal strips
	if(m1x2_model)
		m1x2_model->init_gl();
	if(m1x4_model)
		m1x4_model->init_gl();
	if(m1x8_model)
		m1x8_model->init_gl();
	if(m1x16_model)
		m1x16_model->init_gl();
	if(m1x32_model)
		m1x32_model->init_gl();

	//Vertical strips
	if(m2x1_model)
		m2x1_model->init_gl();
	if(m4x1_model)
		m4x1_model->init_gl();
	if(m8x1_model)
		m8x1_model->init_gl();
	if(m16x1_model)
		m16x1_model->init_gl();
	if(m32x1_model)
		m32x1_model->init_gl();
}
int Window_Model_Holder::term_gl()
{
	if(tile_model)
		tile_model->term_gl();
	if(m2x2_model)
		m2x2_model->term_gl();
	if(m4x4_model)
		m4x4_model->term_gl();
	if(m8x8_model)
		m8x8_model->term_gl();
	if(m16x16_model)
		m16x16_model->term_gl();
	if(m32x32_model)
		m32x32_model->term_gl();

	//Horizontal strips
	if(m1x2_model)
		m1x2_model->term_gl();
	if(m1x4_model)
		m1x4_model->term_gl();
	if(m1x8_model)
		m1x8_model->term_gl();
	if(m1x16_model)
		m1x16_model->term_gl();
	if(m1x32_model)
		m1x32_model->term_gl();

	//Vertical strips
	if(m2x1_model)
		m2x1_model->term_gl();
	if(m4x1_model)
		m4x1_model->term_gl();
	if(m8x1_model)
		m8x1_model->term_gl();
	if(m16x1_model)
		m16x1_model->term_gl();
	if(m32x1_model)
		m32x1_model->term_gl();
}


//Methoids for Interior_Window_Model_Holder

Interior_Window_Model_Holder::Interior_Window_Model_Holder(const char* filepath)
{
	tile_model = new Static_Model(filepath);
	if(!tile_model)
	{
		LOGE("Window Model Holder Error: model failed to load, aborting mesh matrix calculation");
		return;
	}

	Mat4 ident = Mat4::IDENTITY();

	//Making a 1x2 strip
	//=================== Making the horizontal strips ==================
	m1x2_model = new Static_Model(tile_model,ident,tile_model,Mat4::TRANSLATE(Vec3(TILE_SIZE,0,0)));
	m1x4_model = new Static_Model(m1x2_model,ident,m1x2_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*2,0,0)));
	m1x8_model = new Static_Model(m1x4_model,ident,m1x4_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*4,0,0)));
	m1x16_model = new Static_Model(m1x8_model,ident,m1x8_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*8,0,0)));
	m1x32_model = new Static_Model(m1x16_model,ident,m1x16_model,Mat4::TRANSLATE(Vec3(TILE_SIZE*16,0,0)));
}

Interior_Window_Model_Holder::~Interior_Window_Model_Holder ()
{
	//Horizontal strips
	delete tile_model;
	delete m1x2_model;
	delete m1x4_model;
	delete m1x8_model;
	delete m1x16_model;
	delete m1x32_model;
}

int Interior_Window_Model_Holder::init_gl()
{
	if(tile_model)
		tile_model->init_gl();

	//Horizontal strips
	if(m1x2_model)
		m1x2_model->init_gl();
	if(m1x4_model)
		m1x4_model->init_gl();
	if(m1x8_model)
		m1x8_model->init_gl();
	if(m1x16_model)
		m1x16_model->init_gl();
	if(m1x32_model)
		m1x32_model->init_gl();
}
int Interior_Window_Model_Holder::term_gl()
{
	if(tile_model)
		tile_model->term_gl();
	//Horizontal strips
	if(m1x2_model)
		m1x2_model->term_gl();
	if(m1x4_model)
		m1x4_model->term_gl();
	if(m1x8_model)
		m1x8_model->term_gl();
	if(m1x16_model)
		m1x16_model->term_gl();
	if(m1x32_model)
		m1x32_model->term_gl();
}