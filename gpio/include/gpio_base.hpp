#pragma once

namespace driver {

class IGpio {
public:
    virtual ~IGpio() noexcept = default;
    virtual void set() noexcept = 0;
    virtual void reset() noexcept = 0;
    virtual bool is_set() const noexcept = 0;
    virtual void toggle() noexcept = 0;
};

}
