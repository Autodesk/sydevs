#pragma once
#ifndef SYDEVS_UNITS_H_
#define SYDEVS_UNITS_H_

#include <sydevs/core/scale.h>

namespace sydevs {


/** 
 * @brief A template which represents Standard International (SI) units
 *        inclusing the base units such as grams, meters, and seconds, and
 *        derived units such as meters-per-second-squared.
 *
 * @details
 * The `units` type can be used to specify an SI unit at compile time as a
 * combination of the 7 base units: grams (g), meters (m), seconds (s), Amperes
 * (A), Kelvin (L), mols (mol), and candelas (cd).
 *
 * A type alias is defined for dimensionless quantities.
 *
 * ~~~
 * no_units
 * ~~~
 *
 * There is also a type alias for each of the base units:
 * 
 * ~~~
 * grams     // Unit of mass
 * meters    // Unit of distance
 * seconds   // Unit of duration
 * amperes   // Unit of electric current
 * kelvins   // Unit of thermodynamic temperature
 * moles     // Unit of amount of substance
 * candelas  // Unit of luminous intensity
 * ~~~
 * 
 * For each base unit, the associated value is defined as a constant. 
 *
 * ~~~
 * _g    // grams()
 * _m    // meters()
 * _s    // seconds()
 * _A    // amperes()
 * _K    // kelvins()
 * _mol  // moles()
 * _cd   // candelas()
 * ~~~

 * There are two ways to expressed SI derived units. The first is using the
 * template constructor, where each template parameter is the exponent of the
 * corresponding base unit.
 *
 * ~~~
 * units<0, 1, -1, 0, 0, 0, 0>  // meters/second (speed)
 * units<0, 1, -2, 0, 0, 0, 0>  // meters/second^2 (acceleration)
 * units<0, 0, 1, 1, 0, 0, 0>   // second*Ampere or coulomb (electric charge)
 * units<0, -2, 0, 0, 0, 0, 1>  // candelas/meter^2 or lux (illuminance)
 * ~~~
 *
 * The second approach employs the base unit values in conjunction with 
 * multiplication and division operators and `decltype`.
 *
 * ~~~
 * decltype(_m/_s)       // meters/second (speed)
 * decltype(_m/_s/_s)    // meters/second^2 (acceleration)
 * decltype(_s*_A)       // second*Ampere or coulomb (electric charge)
 * decltype (_cd/_m/_m)  // candelas/meter^2 or lux (illuminance)
 * ~~~
 *
 * It is important to note that the `units` class treats grams, not kilograms,
 * as the base unit for mass. This affects derived quantities such as Newtons.
 *
 * ~~~
 * decltype(_g*_m/_s/_s)  // grams*meters/second^2 or milliNewtons (force)
 * ~~~
 */
template<int64 g, int64 m, int64 s, int64 A, int64 K, int64 mol, int64 cd>
struct units
{
    constexpr units() = default;  ///< Constructs a `unit` object.

    enum {
        g_ = g,      ///< Grams exponent
        m_ = m,      ///< Meters exponent
        s_ = s,      ///< Seconds exponent
        A_ = A,      ///< Amperes exponent
        K_ = K,      ///< Kelvins exponent
        mol_ = mol,  ///< Moles exponent
        cd_ = cd,    ///< Candelas exponent
    };

    /**
     * @brief Combines two units into one as if the unit on the left were
     *        multiplied by that on the right.
     */
    template<int64 g_, int64 m_, int64 s_, int64 A_, int64 K_, int64 mol_, int64 cd_>
    constexpr units<g + g_, m + m_, s + s_, A + A_, K + K_, mol + mol_, cd + cd_> operator*(units<g_, m_, s_, A_, K_, mol_, cd_> rhs) const
    {
        return units<g + g_, m + m_, s + s_, A + A_, K + K_, mol + mol_, cd + cd_>();
    }

    /**
     * @brief Combines two units into one as if the unit on the left were
     *        divided by that on the right.
     */
    template<int64 g_, int64 m_, int64 s_, int64 A_, int64 K_, int64 mol_, int64 cd_>
    constexpr units<g - g_, m - m_, s - s_, A - A_, K - K_, mol - mol_, cd - cd_> operator/(units<g_, m_, s_, A_, K_, mol_, cd_> rhs) const
    {
        return units<g - g_, m - m_, s - s_, A - A_, K - K_, mol - mol_, cd - cd_>();
    }
};


using no_units = units<0, 0, 0, 0, 0, 0, 0>;
using grams = units<1, 0, 0, 0, 0, 0, 0>;
using meters = units<0, 1, 0, 0, 0, 0, 0>;
using seconds = units<0, 0, 1, 0, 0, 0, 0>;
using amperes = units<0, 0, 0, 1, 0, 0, 0>;
using kelvins = units<0, 0, 0, 0, 1, 0, 0>;
using moles = units<0, 0, 0, 0, 0, 1, 0>;
using candelas = units<0, 0, 0, 0, 0, 0, 1>;


constexpr auto _1 = no_units();
constexpr auto _g = grams();
constexpr auto _m = meters();
constexpr auto _s = seconds();
constexpr auto _A = amperes();
constexpr auto _K = kelvins();
constexpr auto _mol = moles();
constexpr auto _cd = candelas();


}  // namespace

#endif
