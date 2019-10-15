class ChangedPosition {
public:
  String position;
  int    wasOccupiedBy;

  ChangedPosition() {}

  ChangedPosition(String _position, int _wasOccupiedBy) {
    position      = _position;
    wasOccupiedBy = _wasOccupiedBy;
  }
};
