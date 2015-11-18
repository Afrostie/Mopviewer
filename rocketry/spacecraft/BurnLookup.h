#pragma once
/**
* This struct is used to form the lookup table which is consulted to see whether the
* BurnSequence to which this lookup table belongs needs to fire a burn event at the current state/step
*/
class BurnLookup {
public:
    BurnLookup(void) {
        this->state =0;
        this->step =0;
    }

    BurnLookup(const BurnLookup& bl) {
        this->state = bl.state;
        this->step = bl.step;
    }

    void fillFromExistingBurnLookup (const BurnLookup & other) {
        if (this != &other) // protect against invalid self-assignment
        {
            this->state = other.state;
            this->step = other.step;
        }
    }

    bool operator == (const BurnLookup & other) {
        if (this->state != other.state) {
            return false;
        }
        if (this->step != other.step) {
            return false;
        }
        return true;
    }

    bool operator  != (const BurnLookup & other) {
        return !(*this==other);
    }

    std::string exportAsXML () {
        std::stringstream out;
        out <<"		<burnlookup>" <<std::endl;
        out <<"			<state>" << this->state <<"</state>" <<std::endl;
        out <<"			<step>" << this->step <<"</step>" <<std::endl;
        out <<"		</burnlookup>" <<std::endl;
        return out.str();
    }

    /**
    * In which state is this burn set to occur
    */
    int state;

    /**
    * On which step is this burn set to occur
    */
    int step;
};