using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{

    public record UserDetailModel(
        string Name,
        string Email,
        string TelephoneNumber,
        DateTime Birthday,
        string Hometown,
        string ImageUrl) : ModelBase
    {
        public string Name { get; set; } = Name;
        public string Email { get; set; } = Email;
        public string TelephoneNumber { get; set; } = TelephoneNumber;
        public DateTime Birthday { get; set; } = Birthday;
        public string Hometown { get; set; } = Hometown;
        public string? ImageUrl { get; set; } = ImageUrl;
        public List<CarDetailModel> Cars { get; init; } = new();

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<UserEntity, UserDetailModel>()
                    .ReverseMap();

            }
        }

        public static UserDetailModel Empty => new(string.Empty, string.Empty, string.Empty, default, string.Empty, string.Empty);
    }
}