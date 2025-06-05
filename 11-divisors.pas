PROGRAM DIVISORS;
{ Tell us about the divisors of 100! }
VAR
    I: INTEGER;
    N: INTEGER;
    NUM: INTEGER;
    MODULA: INTEGER;

BEGIN

    I := 1;
    WRITE('What number would you like to test?');
    READ(NUM);
    WRITE('The divisors of your number are:');

    WHILE (I<NUM+1)
    BEGIN
        MODULA := NUM;
        WHILE (MODULA > I)
           MODULA := MODULA - I;
        IF -MODULA = -I THEN
            WRITE(I);
        I := I + 1
    END;  
    
    WRITE('--- Sir, your divisors are ready ---')
END
