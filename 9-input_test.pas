PROGRAM INPUT1;
{ This program tests an input number. }
VAR
  COUNT: INTEGER;
  USER: INTEGER;
  N : INTEGER;
BEGIN
  COUNT := 0;
  { Get the number }
  WRITE('Enter an integer, please:');
  READ(USER);
  N := USER;
  { Count the digits }
  WHILE (N > 0.99999999) 
  BEGIN 
    N := N / 10; 
    COUNT := COUNT + 1
  END;
  { Tell us about the digits }
  WRITE(USER);
  WRITE('contains');
  WRITE(COUNT);
  WRITE('digits');
  WRITE(' ');
  { Comment on a big number! }
  IF NOT (COUNT < 9) THEN
    WRITE('Now, that is a big number!')
END
