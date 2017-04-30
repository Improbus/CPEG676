class cell{
    bool is_bomb;  // true if the cell has a bomb, false if not
    bool is_marked; // whether the cell has been chosen or not
    char flag; //value at cells that aren't bombs, default '_'
    public:
        cell(){
            is_bomb = false;
            is_marked = false;
            flag = '_';
        }
        bool get_is_bomb(){
            return is_bomb;
        }
        bool get_is_marked(){
            return is_marked;
        }
        char get_flag(){
            return flag;
        }
        void set_bomb(bool newBomb){
            is_bomb = newBomb;
        }
        void set_marked(bool newMark){
            is_marked = newMark;
        }
        void set_flag(int newFlag){
            flag = newFlag;
        }
        ~cell(){
            
        }
};