/*Modified from github.com/ArashPartow/bitmap (MIT License)*/


#ifndef INCLUDE_BITMAP_IMAGE_HPP
#define INCLUDE_BITMAP_IMAGE_HPP

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>


class bitmap_image
{
public:

   enum channel_mode {
                        rgb_mode = 0,
                        bgr_mode = 1
                     };

   enum color_plane {
                       blue_plane  = 0,
                       green_plane = 1,
                       red_plane   = 2
                    };

   struct rgb_t
   {
      unsigned char   red;
      unsigned char green;
      unsigned char  blue;
   };

   bitmap_image()
   : file_name_(""),
     width_          (0),
     height_         (0),
     row_increment_  (0),
     bytes_per_pixel_(3),
     channel_mode_(bgr_mode)
   {}

   bitmap_image(const std::string& filename)
   : file_name_(filename),
     width_          (0),
     height_         (0),
     row_increment_  (0),
     bytes_per_pixel_(0),
     channel_mode_(bgr_mode)
   {
      load_bitmap();
   }

   bitmap_image(const unsigned int width, const unsigned int height)
   : file_name_(""),
     width_ (width ),
     height_(height),
     row_increment_  (0),
     bytes_per_pixel_(3),
     channel_mode_(bgr_mode)
   {
      create_bitmap();
   }

   bitmap_image(const bitmap_image& image)
   : file_name_(image.file_name_),
     width_    (image.width_    ),
     height_   (image.height_   ),
     row_increment_  (0),
     bytes_per_pixel_(3),
     channel_mode_(bgr_mode)
   {
      create_bitmap();
      data_ = image.data_;
   }

   bitmap_image& operator=(const bitmap_image& image)
   {
      if (this != &image)
      {
         file_name_       = image.file_name_;
         bytes_per_pixel_ = image.bytes_per_pixel_;
         width_           = image.width_;
         height_          = image.height_;
         row_increment_   = 0;
         channel_mode_    = image.channel_mode_;
         create_bitmap();
         data_ = image.data_;
      }

      return *this;
   }

   inline bool operator!()
   {
      return (data_.size()   == 0) ||
             (width_         == 0) ||
             (height_        == 0) ||
             (row_increment_ == 0);
   }

   inline void clear(const unsigned char v = 0x00)
   {
      std::fill(data_.begin(), data_.end(), v);
   }

   inline unsigned char red_channel(const unsigned int x, const unsigned int y) const
   {
      return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)];
   }

   inline unsigned char green_channel(const unsigned int x, const unsigned int y) const
   {
      return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)];
   }

   inline unsigned char blue_channel (const unsigned int x, const unsigned int y) const
   {
      return data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)];
   }

   inline void red_channel(const unsigned int x, const unsigned int y, const unsigned char value)
   {
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 2)] = value;
   }

   inline void green_channel(const unsigned int x, const unsigned int y, const unsigned char value)
   {
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 1)] = value;
   }

   inline void blue_channel (const unsigned int x, const unsigned int y, const unsigned char value)
   {
      data_[(y * row_increment_) + (x * bytes_per_pixel_ + 0)] = value;
   }

   inline unsigned char* row(unsigned int row_index) const
   {
      return const_cast<unsigned char*>(&data_[(row_index * row_increment_)]);
   }

   inline void get_pixel(const unsigned int x, const unsigned int y,
                         unsigned char& red,
                         unsigned char& green,
                         unsigned char& blue) const
   {
      const unsigned int y_offset = y * row_increment_;
      const unsigned int x_offset = x * bytes_per_pixel_;
      const unsigned int offset   = y_offset + x_offset;

      blue  = data_[offset + 0];
      green = data_[offset + 1];
      red   = data_[offset + 2];
   }

   template <typename RGB>
   inline void get_pixel(const unsigned int x, const unsigned int y, RGB& colour) const
   {
      get_pixel(x, y, colour.red, colour.green, colour.blue);
   }

   inline rgb_t get_pixel(const unsigned int x, const unsigned int y) const
   {
      rgb_t colour;
      get_pixel(x, y, colour.red, colour.green, colour.blue);
      return colour;
   }

   inline void set_pixel(const unsigned int x, const unsigned int y,
                         const unsigned char red,
                         const unsigned char green,
                         const unsigned char blue)
   {
      const unsigned int y_offset = y * row_increment_;
      const unsigned int x_offset = x * bytes_per_pixel_;
      const unsigned int offset   = y_offset + x_offset;

      data_[offset + 0] = blue;
      data_[offset + 1] = green;
      data_[offset + 2] = red;
   }

   template <typename RGB>
   inline void set_pixel(const unsigned int x, const unsigned int y, const RGB& colour)
   {
      set_pixel(x, y, colour.red, colour.green, colour.blue);
   }

   inline unsigned int width() const
   {
      return width_;
   }

   inline unsigned int height() const
   {
      return height_;
   }

   inline unsigned int bytes_per_pixel() const
   {
      return bytes_per_pixel_;
   }

   inline unsigned int pixel_count() const
   {
      return width_ *  height_;
   }

   inline void setwidth_height(const unsigned int width,
                               const unsigned int height,
                               const bool clear = false)
   {
      data_.clear();
      width_  = width;
      height_ = height;

      create_bitmap();

      if (clear)
      {
         std::fill(data_.begin(), data_.end(), static_cast<unsigned char>(0x00));
      }
   }

   void save_image(const std::string& file_name) const
   {
      std::ofstream stream(file_name.c_str(),std::ios::binary);

      if (!stream)
      {
         std::cerr << "bitmap_image::save_image(): Error - Could not open file "
                   << file_name << " for writing!" << std::endl;
         return;
      }

      bitmap_information_header bih;

      bih.width            = width_;
      bih.height           = height_;
      bih.bit_count        = static_cast<unsigned short>(bytes_per_pixel_ << 3);
      bih.clr_important    = 0;
      bih.clr_used         = 0;
      bih.compression      = 0;
      bih.planes           = 1;
      bih.size             = bih.struct_size();
      bih.x_pels_per_meter = 0;
      bih.y_pels_per_meter = 0;
      bih.size_image       = (((bih.width * bytes_per_pixel_) + 3) & 0x0000FFFC) * bih.height;

      bitmap_file_header bfh;

      bfh.type             = 19778;
      bfh.size             = bfh.struct_size() + bih.struct_size() + bih.size_image;
      bfh.reserved1        = 0;
      bfh.reserved2        = 0;
      bfh.off_bits         = bih.struct_size() + bfh.struct_size();

      write_bfh(stream,bfh);
      write_bih(stream,bih);

      unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
      char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

      for (unsigned int i = 0; i < height_; ++i)
      {
         const unsigned char* data_ptr = &data_[(row_increment_ * (height_ - i - 1))];

         stream.write(reinterpret_cast<const char*>(data_ptr), sizeof(unsigned char) * bytes_per_pixel_ * width_);
         stream.write(padding_data,padding);
      }

      stream.close();
   }

   inline void set_all_ith_bits_low(const unsigned int bitr_index)
   {
      unsigned char mask = static_cast<unsigned char>(~(1 << bitr_index));

      for (unsigned char* itr = data(); itr != end(); ++itr)
      {
         *itr &= mask;
      }
   }

   inline void set_all_ith_bits_high(const unsigned int bitr_index)
   {
      unsigned char mask = static_cast<unsigned char>(1 << bitr_index);

      for (unsigned char* itr = data(); itr != end(); ++itr)
      {
         *itr |= mask;
      }
   }

   inline void set_all_ith_channels(const unsigned int& channel, const unsigned char& value)
   {
      for (unsigned char* itr = (data() + channel); itr < end(); itr += bytes_per_pixel_)
      {
         *itr = value;
      }
   }

   inline void set_channel(const color_plane color,const unsigned char& value)
   {
      for (unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
      {
         *itr = value;
      }
   }

   inline void ror_channel(const color_plane color, const unsigned int& ror)
   {
      for (unsigned char* itr = (data() + offset(color)); itr < end(); itr += bytes_per_pixel_)
      {
         *itr = static_cast<unsigned char>(((*itr) >> ror) | ((*itr) << (8 - ror)));
      }
   }

   inline void set_all_channels(const unsigned char& value)
   {
      for (unsigned char* itr = data(); itr < end(); )
      {
         *(itr++) = value;
      }
   }

   inline void set_all_channels(const unsigned char& r_value,
                                const unsigned char& g_value,
                                const unsigned char& b_value)
   {
      for (unsigned char* itr = (data() + 0); itr < end(); itr += bytes_per_pixel_)
      {
         *(itr + 0) = b_value;
         *(itr + 1) = g_value;
         *(itr + 2) = r_value;
      }
   }


   inline const unsigned char* data() const
   {
      return data_.data();
   }

   inline unsigned char* data()
   {
      return const_cast<unsigned char*>(data_.data());
   }


   inline unsigned int offset(const color_plane color) const
   {
      switch (channel_mode_)
      {
         case rgb_mode : {
                            switch (color)
                            {
                               case red_plane   : return 0;
                               case green_plane : return 1;
                               case blue_plane  : return 2;
                               default          : return std::numeric_limits<unsigned int>::max();
                            }
                         }

         case bgr_mode : {
                            switch (color)
                            {
                               case red_plane   : return 2;
                               case green_plane : return 1;
                               case blue_plane  : return 0;
                               default          : return std::numeric_limits<unsigned int>::max();
                            }
                         }

         default       : return std::numeric_limits<unsigned int>::max();
      }
   }

   inline void reverse_channels()
   {
      if (3 != bytes_per_pixel_)
         return;

      for (unsigned char* itr = data(); itr < end(); itr += bytes_per_pixel_)
      {
         std::swap(*(itr + 0),*(itr + 2));
      }
   }

private:

   inline const unsigned char* end() const
   {
      return data_.data() + data_.size();
   }

   inline unsigned char* end()
   {
      return const_cast<unsigned char*>(data() + data_.size());
   }

   struct bitmap_file_header
   {
      unsigned short type;
      unsigned int   size;
      unsigned short reserved1;
      unsigned short reserved2;
      unsigned int   off_bits;

      unsigned int struct_size() const
      {
         return sizeof(type     ) +
                sizeof(size     ) +
                sizeof(reserved1) +
                sizeof(reserved2) +
                sizeof(off_bits ) ;
      }

      void clear()
      {
         std::memset(this, 0x00, sizeof(bitmap_file_header));
      }
   };

   struct bitmap_information_header
   {
      unsigned int   size;
      unsigned int   width;
      unsigned int   height;
      unsigned short planes;
      unsigned short bit_count;
      unsigned int   compression;
      unsigned int   size_image;
      unsigned int   x_pels_per_meter;
      unsigned int   y_pels_per_meter;
      unsigned int   clr_used;
      unsigned int   clr_important;

      unsigned int struct_size() const
      {
         return sizeof(size            ) +
                sizeof(width           ) +
                sizeof(height          ) +
                sizeof(planes          ) +
                sizeof(bit_count       ) +
                sizeof(compression     ) +
                sizeof(size_image      ) +
                sizeof(x_pels_per_meter) +
                sizeof(y_pels_per_meter) +
                sizeof(clr_used        ) +
                sizeof(clr_important   ) ;
      }

      void clear()
      {
         std::memset(this, 0x00, sizeof(bitmap_information_header));
      }
   };

   inline bool big_endian() const
   {
      unsigned int v = 0x01;

      return (1 != reinterpret_cast<char*>(&v)[0]);
   }

   inline unsigned short flip(const unsigned short& v) const
   {
      return ((v >> 8) | (v << 8));
   }

   inline unsigned int flip(const unsigned int& v) const
   {
      return (
               ((v & 0xFF000000) >> 0x18) |
               ((v & 0x000000FF) << 0x18) |
               ((v & 0x00FF0000) >> 0x08) |
               ((v & 0x0000FF00) << 0x08)
             );
   }

   template <typename T>
   inline void read_from_stream(std::ifstream& stream,T& t)
   {
      stream.read(reinterpret_cast<char*>(&t),sizeof(T));
   }

   template <typename T>
   inline void write_to_stream(std::ofstream& stream,const T& t) const
   {
      stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
   }

   inline void read_bfh(std::ifstream& stream, bitmap_file_header& bfh)
   {
      read_from_stream(stream,bfh.type     );
      read_from_stream(stream,bfh.size     );
      read_from_stream(stream,bfh.reserved1);
      read_from_stream(stream,bfh.reserved2);
      read_from_stream(stream,bfh.off_bits );

      if (big_endian())
      {
         bfh.type      = flip(bfh.type     );
         bfh.size      = flip(bfh.size     );
         bfh.reserved1 = flip(bfh.reserved1);
         bfh.reserved2 = flip(bfh.reserved2);
         bfh.off_bits  = flip(bfh.off_bits );
      }
   }

   inline void write_bfh(std::ofstream& stream, const bitmap_file_header& bfh) const
   {
      if (big_endian())
      {
         write_to_stream(stream,flip(bfh.type     ));
         write_to_stream(stream,flip(bfh.size     ));
         write_to_stream(stream,flip(bfh.reserved1));
         write_to_stream(stream,flip(bfh.reserved2));
         write_to_stream(stream,flip(bfh.off_bits ));
      }
      else
      {
         write_to_stream(stream,bfh.type     );
         write_to_stream(stream,bfh.size     );
         write_to_stream(stream,bfh.reserved1);
         write_to_stream(stream,bfh.reserved2);
         write_to_stream(stream,bfh.off_bits );
      }
   }

   inline void read_bih(std::ifstream& stream,bitmap_information_header& bih)
   {
      read_from_stream(stream,bih.size            );
      read_from_stream(stream,bih.width           );
      read_from_stream(stream,bih.height          );
      read_from_stream(stream,bih.planes          );
      read_from_stream(stream,bih.bit_count       );
      read_from_stream(stream,bih.compression     );
      read_from_stream(stream,bih.size_image      );
      read_from_stream(stream,bih.x_pels_per_meter);
      read_from_stream(stream,bih.y_pels_per_meter);
      read_from_stream(stream,bih.clr_used        );
      read_from_stream(stream,bih.clr_important   );

      if (big_endian())
      {
         bih.size          = flip(bih.size               );
         bih.width         = flip(bih.width              );
         bih.height        = flip(bih.height             );
         bih.planes        = flip(bih.planes             );
         bih.bit_count     = flip(bih.bit_count          );
         bih.compression   = flip(bih.compression        );
         bih.size_image    = flip(bih.size_image         );
         bih.x_pels_per_meter = flip(bih.x_pels_per_meter);
         bih.y_pels_per_meter = flip(bih.y_pels_per_meter);
         bih.clr_used      = flip(bih.clr_used           );
         bih.clr_important = flip(bih.clr_important      );
      }
   }

   inline void write_bih(std::ofstream& stream, const bitmap_information_header& bih) const
   {
      if (big_endian())
      {
         write_to_stream(stream,flip(bih.size            ));
         write_to_stream(stream,flip(bih.width           ));
         write_to_stream(stream,flip(bih.height          ));
         write_to_stream(stream,flip(bih.planes          ));
         write_to_stream(stream,flip(bih.bit_count       ));
         write_to_stream(stream,flip(bih.compression     ));
         write_to_stream(stream,flip(bih.size_image      ));
         write_to_stream(stream,flip(bih.x_pels_per_meter));
         write_to_stream(stream,flip(bih.y_pels_per_meter));
         write_to_stream(stream,flip(bih.clr_used        ));
         write_to_stream(stream,flip(bih.clr_important   ));
      }
      else
      {
         write_to_stream(stream,bih.size            );
         write_to_stream(stream,bih.width           );
         write_to_stream(stream,bih.height          );
         write_to_stream(stream,bih.planes          );
         write_to_stream(stream,bih.bit_count       );
         write_to_stream(stream,bih.compression     );
         write_to_stream(stream,bih.size_image      );
         write_to_stream(stream,bih.x_pels_per_meter);
         write_to_stream(stream,bih.y_pels_per_meter);
         write_to_stream(stream,bih.clr_used        );
         write_to_stream(stream,bih.clr_important   );
      }
   }

   inline std::size_t file_size(const std::string& file_name) const
   {
      std::ifstream file(file_name.c_str(),std::ios::in | std::ios::binary);
      if (!file) return 0;
      file.seekg (0, std::ios::end);
      return static_cast<std::size_t>(file.tellg());
   }

   void create_bitmap()
   {
      row_increment_ = width_ * bytes_per_pixel_;
      data_.resize(height_ * row_increment_);
   }

   void load_bitmap()
   {
      std::ifstream stream(file_name_.c_str(),std::ios::binary);

      if (!stream)
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - "
                   << "file " << file_name_ << " not found!" << std::endl;
         return;
      }

      width_  = 0;
      height_ = 0;

      bitmap_file_header bfh;
      bitmap_information_header bih;

      bfh.clear();
      bih.clear();

      read_bfh(stream,bfh);
      read_bih(stream,bih);

      if (bfh.type != 19778)
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - "
                   << "Invalid type value " << bfh.type << " expected 19778." << std::endl;

         bfh.clear();
         bih.clear();

         stream.close();

         return;
      }

      if (bih.bit_count != 24)
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - "
                   << "Invalid bit depth " << bih.bit_count << " expected 24." << std::endl;

         bfh.clear();
         bih.clear();

         stream.close();

         return;
      }

      if (bih.size != bih.struct_size())
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - "
                   << "Invalid BIH size " << bih.size
                   << " expected " << bih.struct_size() << std::endl;

         bfh.clear();
         bih.clear();

         stream.close();

         return;
      }

      width_  = bih.width;
      height_ = bih.height;

      bytes_per_pixel_ = bih.bit_count >> 3;

      unsigned int padding = (4 - ((3 * width_) % 4)) % 4;
      char padding_data[4] = { 0x00, 0x00, 0x00, 0x00 };

      std::size_t bitmap_file_size = file_size(file_name_);

      std::size_t bitmap_logical_size = (height_ * width_ * bytes_per_pixel_) +
                                        (height_ * padding)                   +
                                         bih.struct_size()                    +
                                         bfh.struct_size()                    ;

      if (bitmap_file_size != bitmap_logical_size)
      {
         std::cerr << "bitmap_image::load_bitmap() ERROR: bitmap_image - "
                   << "Mismatch between logical and physical sizes of bitmap. "
                   << "Logical: "  << bitmap_logical_size << " "
                   << "Physical: " << bitmap_file_size    << std::endl;

         bfh.clear();
         bih.clear();

         stream.close();

         return;
      }

      create_bitmap();

      for (unsigned int i = 0; i < height_; ++i)
      {
         unsigned char* data_ptr = row(height_ - i - 1); // read in inverted row order

         stream.read(reinterpret_cast<char*>(data_ptr), sizeof(char) * bytes_per_pixel_ * width_);
         stream.read(padding_data,padding);
      }
   }

   template <typename T>
   inline T clamp(const T& v, const T& lower_range, const T& upper_range) const
   {
      if (v < lower_range)
         return lower_range;
      else if (v >  upper_range)
         return upper_range;
      else
         return v;
   }

   std::string  file_name_;
   unsigned int width_;
   unsigned int height_;
   unsigned int row_increment_;
   unsigned int bytes_per_pixel_;
   channel_mode channel_mode_;
   std::vector<unsigned char> data_;
};

typedef bitmap_image::rgb_t rgb_t;

inline bool operator==(const rgb_t& c0, const rgb_t& c1)
{
   return (c0.red   == c1  .red) &&
          (c0.green == c1.green) &&
          (c0.blue  == c1 .blue) ;
}

inline bool operator!=(const rgb_t& c0, const rgb_t& c1)
{
   return (c0.red   != c1  .red) ||
          (c0.green != c1.green) ||
          (c0.blue  != c1 .blue) ;
}

inline std::size_t hamming_distance(const rgb_t& c0, const rgb_t& c1)
{
   std::size_t result = 0;

   if (c0.red   != c1  .red) ++result;
   if (c0.green != c1.green) ++result;
   if (c0.blue  != c1 .blue) ++result;

   return result;
}

inline rgb_t make_colour(const unsigned int& red, const unsigned int& green, const unsigned int& blue)
{
   rgb_t result;

   result.red   = static_cast<unsigned char>(red  );
   result.green = static_cast<unsigned char>(green);
   result.blue  = static_cast<unsigned char>(blue );

   return result;
}

template <typename OutputIterator>
inline void generate_colours(const std::size_t& steps, const rgb_t c0, const rgb_t& c1, OutputIterator out)
{
   double dr = ((double)c1.red   -  (double)c0.red   ) / steps;
   double dg = ((double)c1.green -  (double)c0.green ) / steps;
   double db = ((double)c1.blue  -  (double)c0.blue  ) / steps;

   for (std::size_t i = 0; i < steps; ++i)
   {
      rgb_t c;

      c.red   = static_cast<unsigned char>(c0.red   + (i * dr));
      c.green = static_cast<unsigned char>(c0.green + (i * dg));
      c.blue  = static_cast<unsigned char>(c0.blue  + (i * db));

      *(out++) = c;
   }
}

class image_drawer
{
public:

   image_drawer(bitmap_image& image)
   : image_(image),
     pen_width_(1),
     pen_color_red_  (0),
     pen_color_green_(0),
     pen_color_blue_ (0)
   {}

   void plot_pen_pixel(int x, int y)
   {
      switch (pen_width_)
      {
         case 1  : plot_pixel(x,y);
                   break;

         case 2  : {
                      plot_pixel(x    , y    );
                      plot_pixel(x + 1, y    );
                      plot_pixel(x + 1, y + 1);
                      plot_pixel(x    , y + 1);
                   }
                   break;

         case  3 : {
                      plot_pixel(x    , y - 1);
                      plot_pixel(x - 1, y - 1);
                      plot_pixel(x + 1, y - 1);

                      plot_pixel(x    , y    );
                      plot_pixel(x - 1, y    );
                      plot_pixel(x + 1, y    );

                      plot_pixel(x    , y + 1);
                      plot_pixel(x - 1, y + 1);
                      plot_pixel(x + 1, y + 1);
                   }
                   break;

         default : plot_pixel(x,y);
                   break;
      }
   }

   void plot_pixel(int x, int y)
   {
      if (
           (x < 0) ||
           (y < 0) ||
           (x >= static_cast<int>(image_.width ())) ||
           (y >= static_cast<int>(image_.height()))
         )
         return;

      image_.set_pixel(x,y,pen_color_red_,pen_color_green_,pen_color_blue_);
   }

   void pen_width(const unsigned int& width)
   {
      if ((width > 0) && (width < 4))
      {
         pen_width_ = width;
      }
   }

   void pen_color(const unsigned char& red,
                  const unsigned char& green,
                  const unsigned char& blue)
   {
      pen_color_red_   = red;
      pen_color_green_ = green;
      pen_color_blue_  = blue;
   }

   template <typename RGB>
   void pen_color(const RGB colour)
   {
      pen_color_red_   = colour.red;
      pen_color_green_ = colour.green;
      pen_color_blue_  = colour.blue;
   }

private:

   image_drawer(const image_drawer& id);
   image_drawer& operator =(const image_drawer& id);

   bitmap_image& image_;
   unsigned int  pen_width_;
   unsigned char pen_color_red_;
   unsigned char pen_color_green_;
   unsigned char pen_color_blue_;
};

class cartesian_canvas
{
public:

   cartesian_canvas(const double x_length, const double y_length)
   : width_div2_ (0.0),
     height_div2_(0.0),
     min_x_      (0.0),
     min_y_      (0.0),
     max_x_      (0.0),
     max_y_      (0.0),
     draw_       (image_)
   {
      setup_canvas(x_length,y_length);
   }

   inline bool operator!()
   {
      return !image_;
   }



   void plot_pen_pixel(double x, double y)
   {
      if ((x < min_x_) || (x > max_x_)) return;
      if ((y < min_y_) || (y > max_y_)) return;

      const int sc_x = static_cast<int>(cart_to_screen_x(x));
      const int sc_y = static_cast<int>(cart_to_screen_y(y));

      draw_.plot_pen_pixel(sc_x, sc_y);
   }

   void plot_pixel(double x, double y)
   {
      if ((x < min_x_) || (x > max_x_)) return;
      if ((y < min_y_) || (y > max_y_)) return;

      const int sc_x = static_cast<int>(cart_to_screen_x(x));
      const int sc_y = static_cast<int>(cart_to_screen_y(y));

      draw_.plot_pixel(sc_x, sc_y);
   }

   void pen_width(const unsigned int& width)
   {
      draw_.pen_width(width);
   }

   void pen_color(const unsigned char&   red,
                  const unsigned char& green,
                  const unsigned char&  blue)
   {
      draw_.pen_color(red,green,blue);
   }

   template <typename RGB>
   void pen_color(const RGB colour)
   {
      draw_.pen_color(colour);
   }

   const bitmap_image& image() const
   {
      return image_;
   }

   bitmap_image& image()
   {
      return image_;
   }

   void set_widthheight(const double x_length, const double y_length)
   {
      setup_canvas(x_length, y_length);
   }

   double min_x() const { return min_x_; }
   double min_y() const { return min_y_; }
   double max_x() const { return max_x_; }
   double max_y() const { return max_y_; }

private:

   void setup_canvas(const double x_length, const double y_length)
   {
      if ((x_length < 2.0) || (y_length < 2.0))
         return;

      width_div2_  = x_length / 2.0;
      height_div2_ = y_length / 2.0;

      min_x_ = -width_div2_ ;
      min_y_ = -height_div2_;
      max_x_ =  width_div2_ ;
      max_y_ =  height_div2_;

      image_.setwidth_height(static_cast<unsigned int>(x_length) + 1, static_cast<unsigned int>(y_length) + 1);

      image_.clear(0xFF);
   }


   double clamp_y(const double& y)
   {
           if (y < min_y_)  return min_y_;
      else if (y > max_y_)  return max_y_;
      else                  return y;
   }

   double cart_to_screen_x(const double& x)
   {
      return x + width_div2_;
   }

   double cart_to_screen_y(const double& y)
   {
      return height_div2_ - y;
   }

   enum clip_code
   {
      e_clip_bottom = 1,
      e_clip_top    = 2,
      e_clip_left   = 4,
      e_clip_right  = 8
   };

   int out_code(
                 const double&  x, const double&  y,
                 const double& x1, const double& y1,
                 const double& x2, const double& y2
               )
   {
      int result = 0;
      if (y < y1)      result |= e_clip_bottom;
      else if (y > y2) result |= e_clip_top;

      if (x < x1)      result |= e_clip_left;
      else if (x > x2) result |= e_clip_right;

      return result;
   }

   bool clip(double& x1, double& y1, double& x2, double& y2)
   {
      bool   result = false;
      double x      = 0.0;
      double y      = 0.0;

      int outcode0   = out_code(x1, y1, min_x_, min_y_, max_x_, max_y_);
      int outcode1   = out_code(x2, y2, min_x_, min_y_, max_x_, max_y_);
      int outcodeout = 0;

      while ((outcode0 != 0) || (outcode1 != 0))
      {
         if ((outcode0 & outcode1) != 0)
            return result;
         else
         {
            if (outcode0 != 0)
               outcodeout = outcode0;
            else
               outcodeout = outcode1;

            double dx = (x2 - x1);
            double dy = (y2 - y1);

            if ((outcodeout & e_clip_bottom) == e_clip_bottom)
            {
               x = x1 + dx * (min_y_ - y1) / dy;
               y = min_y_;
            }
            else if ((outcodeout & e_clip_top) == e_clip_top)
            {
               x = x1 + dx * (max_y_ - y1) / dy;
               y = max_y_;
            }
            else if ((outcodeout & e_clip_right) == e_clip_right)
            {
               y = y1 + dy * (max_x_ - x1) / dx;
               x = max_x_;
            }
            else if ((outcodeout & e_clip_left) == e_clip_left)
            {
               y = y1 + dy * (min_x_ - x1) / dx;
               x = min_x_;
            }

            if (outcodeout == outcode0)
            {
               x1 = x;
               y1 = y;
               outcode0 = out_code(x1, y1, min_x_, min_y_, max_x_, max_y_);
            }
            else
            {
               x2 = x;
               y2 = y;
               outcode1 = out_code(x2, y2, min_x_, min_y_, max_x_, max_y_);
            }
         }
      }

      return true;
   }

   cartesian_canvas(const cartesian_canvas&);
   cartesian_canvas operator=(const cartesian_canvas&);

   double width_div2_;
   double height_div2_;
   double min_x_;
   double min_y_;
   double max_x_;
   double max_y_;
   bitmap_image image_;
   image_drawer draw_;
};



#endif
