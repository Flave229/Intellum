#ifndef _INPUT_H_
#define _INPUT_H_

class Input
{
// Member Level Variables
private:
	bool _keys[256];

// Function Declarations
public:
	Input();
	Input(const Input&);
	~Input();

	void Initialise();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
};

#endif