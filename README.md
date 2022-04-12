# Maoutch
RPG Match Game made with SFML in C++

## Game Loop when swipe
Actual:
- Swap
- Timer > ProcessMatch^
- Timer > DestroyMatches
- if (Destroyed) Timer > CollapseColumns
    - Timer > RefillColumn
    - Timer > AfterRefillColumn
    - if (pseudoMatch found in the grid) Timer > ProcessMatch^
    - else Timer > ProcessPossibleMatches
        - if (possibleMatches not found) Timer > StartReset
            - Timer > Reset
- else Timer > SwapBack

Wanted:
- Swap
- Timer > ProcessMatch^
- Timer > DestroyMatches
- Timer > ProcessSpecialEffects^
- if (Destroyed) Timer > CollapseColumns
    - Timer > RefillColumn
    - Timer > AfterRefillColumn
    - if (nedd process special effect)
        - Timer > ProcessSpecialEffects^
    - else
        - if (pseudoMatch found in the grid) Timer > ProcessMatch^
        - else Timer > ProcessPossibleMatches
            - if (possibleMatches not found) Timer > StartReset
                - Timer > Reset
- else Timer > SwapBack

## Credits
- Easing Functions - https://github.com/nicolausYes/easing-functions
- Json Parser - https://github.com/nlohmann/json