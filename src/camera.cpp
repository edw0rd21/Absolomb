#include "camera.h"

void camera::render(const hittable& world)
{
    //start clock
    auto start = std::chrono::high_resolution_clock::now();

    initialize();

    //logging
    int total_pixels = image_width * image_height;
    int processed_pixels = 0;

    ThreadPool pool(std::thread::hardware_concurrency());

    //raytracing(render) loop
    for (int j = 0; j < image_height; j++)
    {
        //std::clog << "\rScanlines remaining: " << (image_height - j) << ' ';

        for (int i = 0; i < image_width; i++)
        {
            pool.enqueue([&, i, j] 
         
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) 
                {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }   

                // Write color to SFML window
                {
                    std::lock_guard<std::mutex> lock(draw_mutex);

                    auto pixels = draw.calColor(pixel_samples_scale * pixel_color); 

                    // Write color to SFML window
                    sf::Color sf_color = { 
                        static_cast<sf::Uint8>(pixels.x()) ,
                        static_cast<sf::Uint8>(pixels.y()) ,
                        static_cast<sf::Uint8>(pixels.z()) };   

                    winrndr.setPixel(i, j, sf_color);

                    processed_pixels++;

                    log_progress(j, processed_pixels, total_pixels);
                }
                             
            });
        }
    }
    
    pool.wait(); 

    //Write color to ppm file
    ppm << "P3\n" << image_width << ' ' << image_height << "\n255\n";
   
    auto& img = winrndr.getImage();
    auto ptr = img.getPixelsPtr();

    for (int n = 0; n < image_height * image_width * 4; n += 4)
    {
        ppm << (int)ptr[n] << ' ' << (int)ptr[n + 1] << ' ' << (int)ptr[n + 2] << '\n' << std::flush;
    }

    ppm.close();    

    //end clock
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    double secs = elapsed.count();
    double mins = secs/60.0;

    std::clog << "\rDone." << std::string(50, ' ');

    if (mins > 6.0)   
        std::clog << "\nRender time: " << secs << " s  /  " << mins << " mins." <<std::endl;    
    else
        std::clog << "\nRender time: " << secs << " seconds." << std::endl;
    
    winrndr.display(); 
}

void camera::initialize()
{
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = lookfrom;


    // Determine viewport dimensions.
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);
    auto viewport_height = 2 * h * focus_dist;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
    vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge


    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    ppm.open("output1.ppm", std::ios::out);

    winrndr.InIt(image_width, image_height);

}

ray camera::get_ray(int i, int j) const
{
    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j.

    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
        + ((i + offset.x()) * pixel_delta_u)
        + ((j + offset.y()) * pixel_delta_v);

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

vec3 camera::sample_square() const
{
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

point3 camera::defocus_disk_sample() const 
{
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

color camera::ray_color(const ray& r, int depth, const hittable& world) const
{
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;

    if (world.hit(r, interval(0.001 , infinity), rec)) 
    {
        
        ray scattered;
        color attenuation;
        double gLit = 0.5;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, depth - 1, world);
        return color(0, 0, 0);


        //return 0.3 * (rec.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

void camera::log_progress(int j, int processed_pixels, int total_pixels) 
{

    if (processed_pixels % 500 != 0)
        return;

    int percentage = static_cast<int>((static_cast<double>(processed_pixels) / total_pixels) * 100);

    std::ostringstream log;

    // Print the progress on the first line
    log << "\rProgress:  "
        << percentage << "%   ("
        << processed_pixels << " of "
        << total_pixels << "  pixels remaining)";   
    
    log << "\nScanlines remaining: " << image_height - j << ' ' ;
    
    std::clog << log.str() << "\033[F" << std::flush;      // Move the cursor up to overwrite the previous scanline update
       
}