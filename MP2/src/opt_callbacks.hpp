//
// Created by Christian J Howard on 10/29/17.
//

#ifndef SRC_OPT_CALLBACKS_HPP
#define SRC_OPT_CALLBACKS_HPP

namespace opt {
    template<typename optimizer>
    class callback {
    public:
        callback()          = default;
        virtual ~callback() = default;
        void setOptimizer(const optimizer& opt_){ optimizer_ = &opt_; }
        virtual void run() = 0;

    protected:
        const optimizer* getOptimizer() const { return optimizer_; }

    private:
        const optimizer* optimizer_;
    };
}

#endif //SRC_OPT_CALLBACKS_HPP
