// Port of LISTING 65 (haversine_math.cpp)

fn square(a: f64) -> f64 {
    a * a
}

fn radians_from_degrees(degrees: f64) -> f64 {
    0.01745329251994329577 * degrees
}

/// NOTE(casey): EarthRadius is generally expected to be 6372.8
///
/// This is not meant to be a "good" way to calculate the Haversine distance.
/// Instead, it attempts to follow, as closely as possible, the formula used in
/// the real-world question on which these homework exercises are loosely based.
pub fn reference_haversine(x0: f64, y0: f64, x1: f64, y1: f64, earth_radius: f64) -> f64 {
    let lat1 = y0;
    let lat2 = y1;
    let lon1 = x0;
    let lon2 = x1;

    let d_lat = radians_from_degrees(lat2 - lat1);
    let d_lon = radians_from_degrees(lon2 - lon1);
    let lat1 = radians_from_degrees(lat1);
    let lat2 = radians_from_degrees(lat2);

    let a = square((d_lat / 2.0).sin())
        + lat1.cos() * lat2.cos() * square((d_lon / 2.0).sin());
    let c = 2.0 * a.sqrt().asin();

    earth_radius * c
}
