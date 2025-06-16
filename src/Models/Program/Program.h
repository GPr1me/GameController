#ifndef GAME_H
#define GAME_H

class Program {
public:
	virtual void setup() = 0;
	virtual void run() = 0;
	virtual ~Program() = default;

	virtual bool isSetup();
	virtual void drawTitleScreen(bool isSelected) = 0;

protected:
	bool _isSetup;
};

#endif // GAME_H