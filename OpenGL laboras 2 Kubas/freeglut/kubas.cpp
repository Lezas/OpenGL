class kubas {
private:
	string vardas;
	char FrontW;
	char BackW;
	char TopW;
	char BottomW;
	char LeftW;
	char RightW;
public:
	kubas::~kubas(){
		this->vardas= "";
		this->BackW=' ';
		this->FrontW=' ';
		this->TopW=' ';
		this->BottomW=' ';
		this->LeftW=' ';
		this->RightW=' ';
	}
	kubas::kubas(string Vardas){
		this->vardas = Vardas;
	}

	void kubas::WallColors(char FrontW, char Backw, char TopW, char BottomW, char LeftW, char RightW){
		this->FrontW = FrontW;
		this->BackW = BackW;
		this->TopW = TopW;
		this->BottomW = BottomW;
		this->LeftW = LeftW;
		this->RightW = RightW;
	}

	void kubas::draw(){
		glPushMatrix();
		GLfloat StartPoint[3] = {0.0,0.0,0.0};
		if(this->vardas.compare("FTL") == false){Copy(StartPoint, FTL, 3);} else 
		if(this->vardas.compare("FTR") == false){Copy(StartPoint, FTR, 3);} else
		if(this->vardas.compare("FBL") == false){Copy(StartPoint, FBL, 3);} else
		if(this->vardas.compare("FBR") == false){Copy(StartPoint, FBR, 3);} else
		if(this->vardas.compare("BTL") == false){Copy(StartPoint, BTL, 3);} else
		if(this->vardas.compare("BTR") == false){Copy(StartPoint, BTR, 3);} else
		if(this->vardas.compare("BBL") == false){Copy(StartPoint, BBL, 3);} else
		if(this->vardas.compare("BBR") == false){Copy(StartPoint, BBR, 3);}

		GLuint spalva;

		spalva = red_textureId;
		switch(this->FrontW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}
		//Front wall
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Back Wall

		spalva = red_textureId;
		switch(this->BackW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Top wall
		spalva = red_textureId;
		switch(this->TopW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		
		//Bottom Wall
		spalva = red_textureId;
		switch(this->BottomW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Right Wall
		spalva = red_textureId;
		switch(this->RightW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0]+2.0, StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//Left Wall
		//spalva = red_textureId;
		switch(this->LeftW){
		case 'G':{ spalva = green_textureId;} break;
		case 'Y':{ spalva = yellow_textureId;} break;
		case 'O':{ spalva = orange_textureId;} break;
		case 'W':{ spalva = white_textureId;} break;
		case 'B':{ spalva = blue_textureId;} break;
		case 'R':{ spalva = red_textureId;} break;
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, spalva);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(StartPoint[0], StartPoint[1], StartPoint[2]-2.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]-2.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(StartPoint[0], StartPoint[1]-2.0, StartPoint[2]);
 
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
};
