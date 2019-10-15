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

  #define CONFIRM_TIME 500

  public:
    String position;
    int    reading;
    bool   value;

    int    status;
    int    occupiedBy;

    bool   changed;
    long   lastChange;

    Position(String newPosition, int newOccupiedBy) {
      position   = newPosition;
      status     = CONFIRMED;
      occupiedBy = newOccupiedBy;
      value      = occupiedBy ? true : false;
      changed    = false;
      lastChange = 0;
    }

    void setNewValue(int newValue, long currentTime) {
      changed    = true;
      value      = newValue;
      lastChange = currentTime;
    }

    void verifyStatus(int upCount, int downCount) {
      if (!changed) return;

      // replacing piece, revert status and changed
      if (value && upCount == 0) {
        status = CONFIRMED;
        changed = false;
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
      changed = false;
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
