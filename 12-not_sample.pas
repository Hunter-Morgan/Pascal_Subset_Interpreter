PROGRAM NOTEX;
{ Testing logical calculations. }
VAR
    A: INTEGER;
    NUM: INTEGER;
BEGIN
    { Modify A to test other truth values. }
    WRITE('A = ');
    READ(NUM);
    A := NUM;
    IF NOT A THEN
    	WRITE('TRUE')
    ELSE
        A := NOT (3<4)
    { Indeed, what is truth? }
END
