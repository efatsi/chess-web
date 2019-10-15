class Position {
  // status
  #define CONFIRMED     0
  #define UNSTABLE_UP   1
  #define STABLE_UP     2
  #define UNSTABLE_DOWN 3
  #define STABLE_DOWN   4

  // occupiedBy
  #define EMPTY 0
  #define WHITE 1
  #define BLACK 2

  #define CONFIRM_TIME 200

  public:
    String position;
    int    status;
    int    occupiedBy;
    int    value;
    long   lastChange;

    Position(String newPosition, int newOccupiedBy) {
      position   = newPosition;
      status     = CONFIRMED;
      occupiedBy = newOccupiedBy;
      value      = occupiedBy ? 1 : 0;
      lastChange = 0;
    }

    void setStatus(int newValue, int upCount, int downCount, long currentTime) {
      value      = newValue;
      lastChange = currentTime;

      if (newValue) {
        if (upCount == 0 && downCount == 1) {
          // changed their mind
          status = CONFIRMED;
        } else {
          status = UNSTABLE_DOWN;
        }
      } else {
        if (occupiedBy == EMPTY) {
          // just passing by
          status = CONFIRMED;
        } else {
          status = UNSTABLE_UP;
        }
      }
    }

    void checkStability(long currentTime) {
      if (_unstable() && _changeIsOld(currentTime)) {
        if (status == UNSTABLE_UP) {
          status = STABLE_UP;
        } else if (status == UNSTABLE_DOWN) {
          status = STABLE_DOWN;
        }
      }
    }

    void confirmState(int player) {
      if (status == STABLE_UP) {
        occupiedBy = EMPTY;
      } else if (status == STABLE_DOWN) {
        occupiedBy = player;
      }

      status = CONFIRMED;
    }

    bool up() {
      return status == UNSTABLE_UP || status == STABLE_UP;
    }

    bool down() {
      return status == UNSTABLE_DOWN || status == STABLE_DOWN;
    }

  private:
    bool _unstable() {
      return status == UNSTABLE_UP || status == UNSTABLE_DOWN;
    }

    bool _changeIsOld(long currentTime) {
      return currentTime - lastChange > CONFIRM_TIME;
    }
};
