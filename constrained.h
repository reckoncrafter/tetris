template <typename T>
class constrained{
    private:
    T val;
    T lowerbound;
    T upperbound;
    public:
    constrained(T l, T u){
        val = l;
        lowerbound = l;
        upperbound = u;
    }

    T operator=(T x){
        if(x < lowerbound){
            this->val = lowerbound;
            return lowerbound;
        }
        if(x > upperbound){
            this->val = upperbound;
            return upperbound;
        }
        this->val = x;
        return x;
    }
    T operator+=(T x){
        if(this->val+x < lowerbound){
            x = lowerbound;
            return lowerbound;
        }
        if(this->val+x > upperbound){
            x = lowerbound;
            return upperbound;
        }
        this->val = this->val+x;
        return this->val+x;
    }
    T operator-=(T x){
        if(this->val-x < lowerbound){
            this->val = lowerbound;
            return lowerbound;
        }
        if(this->val-x > upperbound){
            this->val = upperbound;
            return upperbound;
        }
        this->val = this->val-x;
        return this->val-x;
    }
    T operator*=(T x){
        if(this->val*x < lowerbound){
            this->val = upperbound;
            return upperbound;            
        }
        if(this->val*x > upperbound){
            this->val = upperbound;
            return upperbound;            
        }
        this->val = this->val*x;
        return this->val*x;
    }
    T operator/=(T x){
        if(this->val/x < lowerbound){
            this->val = upperbound;
            return upperbound;            
        }
        if(this->val/x > upperbound){
            this->val = upperbound;
            return upperbound;            
        }
        this->val = this->val/x;
        return this->val/x;
    }
    operator int(){
        return val;
    }
    operator float(){
        return val;
    }
    operator double(){
        return val;
    }
};