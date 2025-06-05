PROGRAM NUMFAC;
VAR
    I: INTEGER;
    N: INTEGER;
    NUM: INTEGER;
    MODULA: INTEGER;

BEGIN

    I := 1;
    N := 1;
    WRITE('What number would you like to test?');
    READ(NUM);

    WHILE (I<NUM+1)
    BEGIN
        N := N * (I);
        I := I + 1
    END;  
    
    WRITE('The factorial of your number is:');
    WRITE(N)
END