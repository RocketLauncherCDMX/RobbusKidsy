#define SPEED_STEP_NUMBER    6

bool new_forwardButton = false;
bool old_forwardButton = false;
bool new_backwardButton = false;
bool old_backwardButton = false;
bool new_leftButton = false;
bool old_leftButton = false;
bool new_rightButton = false;
bool old_rightButton = false;

int new_joystickPad = false;
int old_joystickPad = false;

bool new_triangleButton = false;
bool old_triangleButton = false;
bool new_crossButton = false;
bool old_crossButton = false;
bool new_squareButton = false;
bool old_squareButton = false;
bool new_circleButton = false;
bool old_circleButton = false;
bool new_selectButton = false;
bool old_selectButton = false;
bool new_startButton = false;
bool old_startButton = false;

int button_forward;
int button_backward;
int button_left;
int button_right;
int joystick_status;

int button_triangle;
int button_cross;
int button_square;
int button_circle;
int button_select;
int button_start;

bool new_angle;
bool old_angle;

void buttonsProcess() {
  new_forwardButton = GamePad.isPressed(UP_BIT);
  new_backwardButton = GamePad.isPressed(DOWN_BIT);
  new_leftButton = GamePad.isPressed(LEFT_BIT);
  new_rightButton = GamePad.isPressed(RIGHT_BIT);
  new_triangleButton = GamePad.isTrianglePressed();
  new_circleButton = GamePad.isCirclePressed();
  new_crossButton = GamePad.isCrossPressed();
  new_squareButton = GamePad.isSquarePressed();
  new_selectButton = GamePad.isSelectPressed();
  new_startButton = GamePad.isStartPressed();  
  
  if(new_forwardButton == false && old_forwardButton == false)  button_forward = HOLD_RELEASED;
  if(new_forwardButton == false && old_forwardButton == true)   button_forward = RELEASED;
  if(new_forwardButton == true && old_forwardButton == false)   button_forward = PRESSED;
  if(new_forwardButton == true && old_forwardButton == true)    button_forward = HOLD_PRESSED;

  if(new_backwardButton == false && old_backwardButton == false)  button_backward = HOLD_RELEASED;
  if(new_backwardButton == false && old_backwardButton == true)   button_backward = RELEASED;
  if(new_backwardButton == true && old_backwardButton == false)   button_backward = PRESSED;
  if(new_backwardButton == true && old_backwardButton == true)    button_backward = HOLD_PRESSED;

  if(new_leftButton == false && old_leftButton == false)  button_left = HOLD_RELEASED;
  if(new_leftButton == false && old_leftButton == true)   button_left = RELEASED;
  if(new_leftButton == true && old_leftButton == false)   button_left = PRESSED;
  if(new_leftButton == true && old_leftButton == true)    button_left = HOLD_PRESSED;

  if(new_rightButton == false && old_rightButton == false)  button_right = HOLD_RELEASED;
  if(new_rightButton == false && old_rightButton == true)   button_right = RELEASED;
  if(new_rightButton == true && old_rightButton == false)   button_right = PRESSED;
  if(new_rightButton == true && old_rightButton == true)    button_right = HOLD_PRESSED;

  if(new_triangleButton == false && old_triangleButton == false)  button_triangle = HOLD_RELEASED;
  if(new_triangleButton == false && old_triangleButton == true)   button_triangle = RELEASED;
  if(new_triangleButton == true && old_triangleButton == false)   button_triangle = PRESSED;
  if(new_triangleButton == true && old_triangleButton == true)    button_triangle = HOLD_PRESSED;

  if(new_crossButton == false && old_crossButton == false)  button_cross = HOLD_RELEASED;
  if(new_crossButton == false && old_crossButton == true)   button_cross = RELEASED;
  if(new_crossButton == true && old_crossButton == false)   button_cross = PRESSED;
  if(new_crossButton == true && old_crossButton == true)    button_cross = HOLD_PRESSED;

  if(new_squareButton == false && old_squareButton == false)  button_square = HOLD_RELEASED;
  if(new_squareButton == false && old_squareButton == true)   button_square = RELEASED;
  if(new_squareButton == true && old_squareButton == false)   button_square = PRESSED;
  if(new_squareButton == true && old_squareButton == true)    button_square = HOLD_PRESSED;

  if(new_circleButton == false && old_circleButton == false)  button_circle = HOLD_RELEASED;
  if(new_circleButton == false && old_circleButton == true)   button_circle = RELEASED;
  if(new_circleButton == true && old_circleButton == false)   button_circle = PRESSED;
  if(new_circleButton == true && old_circleButton == true)    button_circle = HOLD_PRESSED;

  if(new_selectButton == false && old_selectButton == false)  button_select = HOLD_RELEASED;
  if(new_selectButton == false && old_selectButton == true)   button_select = RELEASED;
  if(new_selectButton == true && old_selectButton == false)   button_select = PRESSED;
  if(new_selectButton == true && old_selectButton == true)    button_select = HOLD_PRESSED;

  if(new_startButton == false && old_startButton == false)  button_start = HOLD_RELEASED;
  if(new_startButton == false && old_startButton == true)   button_start = RELEASED;
  if(new_startButton == true && old_startButton == false)   button_start = PRESSED;
  if(new_startButton == true && old_startButton == true)    button_start = HOLD_PRESSED;

  old_forwardButton = new_forwardButton;
  old_backwardButton = new_backwardButton;
  old_leftButton = new_leftButton;
  old_rightButton = new_rightButton;
  old_triangleButton = new_triangleButton;
  old_crossButton = new_crossButton;
  old_squareButton = new_squareButton;
  old_circleButton = new_circleButton;
  old_selectButton = new_selectButton;
  old_startButton = new_startButton;
}
