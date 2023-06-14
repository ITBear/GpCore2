#pragma once

#include "../Types/Containers/GpContainersT.hpp"

namespace GPlatform::Algo {

template<typename T>
class DistributeProportional
{
public:
    static std::vector<T>   SDist   (const std::vector<T>&  aWeights,
                                     const T                aValueToDist);
};

template<typename T>
std::vector<T>  DistributeProportional<T>::SDist
(
    const std::vector<T>&   aWeights,
    const T                 aValueToDist
)
{
    using BigNumT = std::conditional_t<std::is_signed_v<T>, s_int_128, u_int_128>;

    static_assert(std::is_integral<T>(), "ElementT must be integral");
    static_assert(sizeof(T) < sizeof(BigNumT));

    //Check count
    const size_t count = aWeights.size();

    if (count == 0)
    {
        return {};
    }

    //Allocate
    std::vector<T> res;
    res.resize(count);

    //Calc weights summ
    BigNumT weightsSumm = BigNumT(0);

    for (const T w: aWeights)
    {
        weightsSumm += BigNumT(w);
    }

    //Calc shift
    BigNumT shift = BigNumT(0);
    if (weightsSumm == BigNumT(0))
    {
        shift       = BigNumT(1);
        weightsSumm = BigNumT(count);
    }

    //Distribute
    T remain = aValueToDist;
    {
        BigNumT valueToDist = BigNumT(aValueToDist);
        for (size_t id = 0; id < count; id++)
        {
            const BigNumT   weight  = BigNumT(aWeights.at(id)) + shift;
            T               distVal = T((weight * valueToDist) / weightsSumm);

            if (distVal > remain)
            {
                distVal = remain;
            }

            res.at(id)   = distVal;
            remain      -= distVal;
        }
    }

    //Distribute remains
    {
        while (remain > T(0))
        {
            for (size_t id = 0; id < count; id++)
            {
                const BigNumT weight = BigNumT(aWeights.at(id)) + shift;

                if (weight == 0)
                {
                    continue;
                }

                remain--;
                res.at(id)++;

                if (remain == 0)
                {
                    break;
                }
            }
        }
    }

    //Check result
    {
        T controlSumm = T(0);

        for (const T s: res)
        {
            controlSumm += s;
        }

        if (controlSumm != aValueToDist)
        {
            throw GpException(u8"Distribute error");
        }
    }

    return res;
}

}//namespace GPlatform::Algo
