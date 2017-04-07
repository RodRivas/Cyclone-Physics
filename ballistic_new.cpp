


/* Changes in the original code (by RodRivas): inserted 2 funcions: angulo() and setAngulo()
that allows the user to choose some angles (in x and z axis) by pressing the numbers 5, 6, 7 and 8 before the shoot
*/

/* (PT)  Versão modificada do arquivo "ballistic.cpp" para implementação de uma 
guinada (nos eixos x e z) no angulo de tiro. Acréscimo das funções: angulo() e setAngulo(). 
Através das teclas "5,6,7 ou 8" o usuário modifica os valores da variável sinCos, 
implementando seno e cosseno (na variável "position" dentro da função render()), antes de o tiro ser disparado.
Obs.: Ainda contém algus bugs
RodRivas
*/

/*
 * The ballistic demo.
 *
 * Part of the Cyclone physics system.
 *
 * Copyright (c) Icosagon 2003. All Rights Reserved.
 *
 * This software is distributed under licence. Use of this software
 * implies agreement with all terms and conditions of the accompanying
 * software licence.
 */

#include <gl/glut.h>
#include <cyclone/cyclone.h>
#include "../app.h"
#include "../timing.h"
#include <iostream>

#include <stdio.h>

/**
 * The main demo class definition.
 */
class BallisticDemo : public Application
{
    enum ShotType
    {
        UNUSED = 0,
        PISTOL,
        ARTILLERY,
        FIREBALL,
        LASER
		
    };

	 enum ProjectilDirection
    {
        NOUSED = 0,
        DIREITA,
		ESQUERDA,
		MAISESQUERDA,
		MAISESQUERDAINDA
		
    };
	 	
    /**
     * Holds a single ammunition round record.
     */
    struct AmmoRound
    {
        cyclone::Particle particle;
        ShotType type;
		ProjectilDirection tipo;
        unsigned startTime;
		
		float sinCos;
		float val;
		float vaiBuscar;

		float angulo()
		{

			    sinCos = val;
				return sinCos;
		};
		
		 
        /** Draws the round. */
        void render()
        {
            cyclone::Vector3 position;
            particle.getPosition(&position);
			angulo();
			

            glColor3f(0, 0, position.y);
            glPushMatrix();
			/**std::cout << sinCos << "\n" << particle.getPosition().y << "\n"; */
			position.x = position.x * cos(sinCos) - position.z * sin(sinCos);
			position.z = position.z * cos(sinCos) - position.z * sin(sinCos);
			glTranslatef(position.x, position.y, position.z);
			glutSolidCube(3);
            glPopMatrix();

			
            glColor3f(0.75, 0.75, 0.75);
            glPushMatrix();
            glTranslatef(position.x, 0, position.z);
            glScalef(1.0f, 0.1f, 1.0f);
            glutSolidCube(4);
            glPopMatrix();
        }
    };

    /**
     * Holds the maximum number of  rounds that can be
     * fired.
     */
    const static unsigned ammoRounds = 16;

    /** Holds the particle data. */
    AmmoRound ammo[ammoRounds];

	/** Holds the current shot type. */
    ShotType currentShotType;

	/** Holds the horizontal degree. */
	ProjectilDirection setProjectilDirection; 

    /** Dispatches a round. */
    void fire();


	void setAngulo();



public:
    /** Creates a new demo object. */
    BallisticDemo();

    /** Returns the window title for the demo. */
    virtual const char* getTitle();

    /** Update the particle positions. */
    virtual void update();

    /** Display the particle positions. */
    virtual void display();

	
	/** Handle a mouse click. */
    virtual void mouse(int button, int state, int x, int y);

    /** Handle a keypress. */
    virtual void key(unsigned char key);
};

// Method definitions
BallisticDemo::BallisticDemo()
: currentShotType(LASER)
{
    // Make all shots unused
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        shot->type = UNUSED;
    }
}


const char* BallisticDemo::getTitle()
{
    return "RodriSoft > Cyclone > Ballistic Demo";
}

// Função que eu criei 


void BallisticDemo::fire()
{
    // Find the first available round.
    AmmoRound *shot;
	AmmoRound *vaiBuscar;
	
    for (shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type == UNUSED) break;
    }

	// If we didn't find a round, then exit - we can't fire.
    if (shot >= ammo+ammoRounds) return;

	
	for (vaiBuscar = ammo; vaiBuscar < ammo+ammoRounds; vaiBuscar++)
    {
        if (vaiBuscar->type == NOUSED) break;
    }

	// If we didn't find a round, then exit - we can't fire.
    if (vaiBuscar >= ammo+ammoRounds) return;

    // Set the properties of the particle
    switch(currentShotType)
    {
    case PISTOL:
        shot->particle.setMass(2.0f); // 2.0kg
        shot->particle.setVelocity(0.0f, 0.0f, 35.0f); // 35m/s
        shot->particle.setAcceleration(0.0f, -1.0f, 0.0f);
        shot->particle.setDamping(0.99f);
		break;

    case ARTILLERY:
        shot->particle.setMass(200.0f); // 200.0kg
        shot->particle.setVelocity(0.0f, 30.0f, 50.0f); // 50m/s
        shot->particle.setAcceleration(0.0f, -20.0f, 0.0f);
        shot->particle.setDamping(0.99f);
        break;

    case FIREBALL:
        shot->particle.setMass(1.0f); // 1.0kg - mostly blast damage
        shot->particle.setVelocity(0.0f, 0.0f, 10.0f); // 5m/s
        shot->particle.setAcceleration(0.0f, 0.6f, 0.0f); // Floats up
        shot->particle.setDamping(0.9f);
        break;

    case LASER:
        // Note that this is the kind of laser bolt seen in films,
        // not a realistic laser beam!
        shot->particle.setMass(0.1f); // 0.1kg - almost no weight
        shot->particle.setVelocity(0.0f, 0.0f, 100.0f); // 100m/s
        shot->particle.setAcceleration(0.0f, 0.0f, 0.0f); // No gravity
        shot->particle.setDamping(0.99f);
        break;
    }

	switch(setProjectilDirection)
    {   
	case DIREITA:
		vaiBuscar->val = 0.2;
		std::cout<< "DIREITA  " << "\n";
		/* std::cout<< mostra_sinCos.sinCos <<"n"; */
		break;

	case ESQUERDA:
		vaiBuscar->val = -0.5;
		std::cout<<"ESQUERDA  " << "\n";
		/* std::cout<< mostra_sinCos.sinCos <<"n"; */
		break;

		case MAISESQUERDA:
		vaiBuscar->val = -1.0;
		std::cout<<"ESQUERDA  " << "\n";
		/* std::cout<< mostra_sinCos.sinCos <<"n"; */
		break;

		case MAISESQUERDAINDA:
		vaiBuscar->val = -2.0;
		std::cout<<"ESQUERDA  " << "\n";
		/* std::cout<< mostra_sinCos.sinCos <<"n"; */
		break;
	}

    // Set the data common to all particle types
    shot->particle.setPosition(0.0f, 2.5f, 4.0f);
    shot->startTime = TimingData::get().lastFrameTimestamp;
    shot->type = currentShotType;
	
    // Clear the force accumulators
    shot->particle.clearAccumulator();
}

void BallisticDemo::update()
{
    // Find the duration of the last frame in seconds
    float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

	// Update the physics of each particle in turn
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {
            // Run the physics
            shot->particle.integrate(duration);

			
			            // Check if the particle is now invalid
            if (shot->particle.getPosition().y < 0.0f ||
                shot->startTime+10000 < TimingData::get().lastFrameTimestamp ||
                shot->particle.getPosition().z > 500.0f)
				
            {
                // We simply set the shot type to be unused, so the
                // memory it occupies can be reused by another shot.
                shot->type = UNUSED;
            }
        }
    }

    Application::update();
}

void BallisticDemo::display()
{
	//Apenas para a cor das linhas do chão ficarem azuis
	cyclone::Vector3 position;
	            
    // Clear the viewport and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(-25.0, 12.0, 0.0,  0.0, 5.0, 22.0,  0.0, 1.0, 0.0);

    // Draw a sphere at the firing point, and add a shadow projected
    // onto the ground plane.
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glutSolidSphere(0.1f, 5, 5);
    glTranslatef(0.0f, -1.5f, 0.0f);
    glColor3f(0.75f, 0.75f, 0.75f);
    glScalef(1.0f, 0.1f, 1.0f);
    glutSolidSphere(0.1f, 5, 5);
	glPopMatrix();


    // Draw some scale lines
    glColor3f(position.x, 0.75f, 0.75f);
    glBegin(GL_LINES);
    for (unsigned i = 0; i < 400; i += 12)
    {
        glVertex3f(-5.0f, 0.0f, i);
        glVertex3f(10.0f, 0.0f, i);
    }
    glEnd();

    // Render each particle in turn
    for (AmmoRound *shot = ammo; shot < ammo+ammoRounds; shot++)
    {
        if (shot->type != UNUSED)
        {

			shot->render();

        }
    }

    

    // Render the description
    glColor3f(0.0f, 0.0f, 0.0f);
    renderText(10.0f, 34.0f,"Click: Fire\n1-4: Select Ammo");

    // Render the name of the current shot type
    switch(currentShotType)
    {
    case PISTOL: renderText(10.0f, 10.0f, "Current Ammo: Pistol"); break;
    case ARTILLERY: renderText(10.0f, 10.0f, "Current Ammo: Artillery"); break;
    case FIREBALL: renderText(10.0f, 10.0f, "Current Ammo: Fireball");
    case LASER: renderText(10.0f, 10.0f, "Current Ammo: Laser"); break;
    }

	switch(setProjectilDirection)
    {
    case DIREITA: renderText(190.0f, 10.0f, "P/ DIREITA"); break;
    case ESQUERDA: renderText(190.0f, 10.0f, "P/ ESQUERDA"); break;
    }
}

void BallisticDemo::mouse(int button, int state, int x, int y)
{
    // Fire the current weapon.
	if (state == GLUT_DOWN)  fire();
}

void BallisticDemo::key(unsigned char key)
{
    switch(key)
    {
    case '1': currentShotType = PISTOL; break;
    case '2': currentShotType = ARTILLERY; break;
    case '3': currentShotType = FIREBALL; break;
    case '4': currentShotType = LASER; break;
	case '5': setProjectilDirection = MAISESQUERDAINDA; break;	
	case '6': setProjectilDirection = MAISESQUERDA; break;
    case '7': setProjectilDirection = ESQUERDA; break;
	case '8': setProjectilDirection = DIREITA; break;
		
   	
    }
}


/**
 * Called by the common demo framework to create an application
 * object (with new) and return a pointer.
 */
Application *getApplication()
{
    return new BallisticDemo();
}
