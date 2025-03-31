#include <iostream>
#include <vector>
#include <random>
#include <time.h>



/*
You are given a locked container represented as a two-dimensional grid of boolean values (true = locked, false = unlocked).
Your task is to write an algorithm that fully unlocks the box, i.e.,
transforms the entire matrix into all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
    - Use the SecureBox public API (toggle, isLocked, getState).
    - Strategically toggle cells to reach a state where all elements are false.
    - Return true if the box remains locked, false if successfully unlocked.
You are not allowed to path or modify the SecureBox class.

Evaluation Criteria:
    - Functional correctness
    - Computational efficiency
    - Code quality, structure, and comments
    - Algorithmic insight and clarity
*/

class SecureBox
{
private:
    std::vector<std::vector<bool>> box;

public:

    //================================================================================
    // Constructor: SecureBox
    // Description: Initializes the secure box with a given size and 
    //              shuffles its state using a pseudo-random number generator 
    //              seeded with current time.
    //================================================================================
    SecureBox(uint32_t y, uint32_t x) : ySize(y), xSize(x)
    {
        rng.seed(time(0));
        box.resize(y);
        for (auto& it : box)
            it.resize(x);
        shuffle();
    }

    //================================================================================
    // Method: toggle
    // Description: Toggles the state at position (x, y) and also all cells in the
    //              same row above and the same column to the left of it.
    //================================================================================
    void toggle(uint32_t y, uint32_t x)
    {
        box[y][x] = !box[y][x];
        for (uint32_t i = 0; i < xSize; i++)
            box[y][i] = !box[y][i];
        for (uint32_t i = 0; i < ySize; i++)
            box[i][x] = !box[i][x];
    }

    //================================================================================
    // Method: isLocked
    // Description: Returns true if any cell 
    //              in the box is true (locked); false otherwise.
    //================================================================================
    bool isLocked()
    {
        for (uint32_t x = 0; x < xSize; x++)
            for (uint32_t y = 0; y < ySize; y++)
                if (box[y][x])
                    return true;

        return false;
    }

    //================================================================================
    // Method: getState
    // Description: Returns a copy of the current state of the box.
    //================================================================================
    std::vector<std::vector<bool>> getState()
    {
        return box;
    }

private:
    std::mt19937_64 rng;
    uint32_t ySize, xSize;

    //================================================================================
    // Method: shuffle
    // Description: Randomly toggles cells in the box to 
    // create an initial locked state.
    //================================================================================
    void shuffle()
    {
        for (uint32_t t = rng() % 1000; t > 0; t--)
            toggle(rng() % ySize, rng() % xSize);
    }
};

//================================================================================
// Function: openBox
// Description: Your task is to implement this function to unlock the SecureBox.
//              Use only the public methods of SecureBox (toggle, getState, isLocked).
//              You must determine the correct sequence of toggle operations to make
//              all values in the box 'false'. The function should return false if
//              the box is successfully unlocked, or true if any cell remains locked.
//================================================================================







bool openBox(uint32_t y, uint32_t x) {

    if (y > x) std::swap(y, x);

    SecureBox box(y, x);

    std::vector<std::vector<bool>> state = box.getState();
    size_t extendedMatrixSize = x * y;

    std::vector<std::vector<int>> extendedMatrix(extendedMatrixSize, std::vector<int>(extendedMatrixSize, 0));
    std::vector<int> B(extendedMatrixSize, 0);
    std::vector<int> X(extendedMatrixSize, 0);

   
    

    for (int i = 0; i < extendedMatrixSize; i++)  //Creating Extended Matrix
    {
        
        int iy = i / x;
        int ix = i % x;

  
        B[i] = state[iy][ix] ? 1 : 0;

        for (int j = 0; j < extendedMatrixSize; j++) 
        {
            
            int jy = j / x;
            int jx = j % x;

            
            if (i == j)  extendedMatrix[i][j] = 1;

           
            if (jy == iy)  extendedMatrix[i][j] = 1;
                

           
            if (jx == ix)  extendedMatrix[i][j] = 1;
                
        }
    }

    
    for (uint32_t col = 0, row = 0; col < extendedMatrixSize && row < extendedMatrixSize; col++) //Gaussian method
    {
       
        for (uint32_t i = row; i < extendedMatrixSize; i++) 
        {
            if (extendedMatrix[i][col]) 
            {
                std::swap(extendedMatrix[i], extendedMatrix[row]);
                std::swap(B[i], B[row]);
                break;
            }
        }
        if (!extendedMatrix[row][col]) continue;  

        
        for (uint32_t i = 0; i < extendedMatrixSize; i++) 
        {
            if (i != row && extendedMatrix[i][col]) {
                for (uint32_t j = 0; j < extendedMatrixSize; j++)
                    extendedMatrix[i][j] ^= extendedMatrix[row][j];
                B[i] ^= B[row];
            }
        }
        row++;
    }

    
    for (int i = extendedMatrixSize - 1; i >= 0; i--)  
    {
        if (extendedMatrix[i][i]) 
        {
            X[i] = B[i];
        }
    }

   

    for (uint32_t i = 0; i < y; i++)  //Solution
    {
        for (uint32_t j = 0; j < x; j++) 
        {
            if (X[i * x + j]) 
            {
                box.toggle(i, j);
            }
        }
    }
    
    while (box.isLocked())  //If Gaussian method don`t work , will try to use iteraktive method
    {
        for (uint32_t i = 0; i < y; i++) 
        {
            for (uint32_t j = 0; j < x; j++) 
            {
                if (state[i][j])
                {  
                    box.toggle(i, j); 
                }
            }
        }


    }

    return box.isLocked();
}







int main(int argc, char* argv[])
{
    uint32_t y = std::atol(argv[1]);
    uint32_t x = std::atol(argv[2]);
    bool state = openBox(y, x);

    if (state)
        std::cout << "BOX: LOCKED!" << std::endl;
    else
        std::cout << "BOX: OPENED!" << std::endl;

    return state;
}
