class ChangeTime {
    private:
        int milliStamp;
        int change;
    public:
        ChangeTime(int input, int readingChange) {
            milliStamp = input;
            change = readingChange;
         }

        int getMilli() {
            return milliStamp;
         }
         int getState() {
            return change;
        }
    };
