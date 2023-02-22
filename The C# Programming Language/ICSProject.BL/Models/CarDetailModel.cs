using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record CarDetailModel(
        string Manufacturer,
        string Type,
        DateTime RegistrationTime,
        uint NumberOfSeats,
        string? ImageUrl,
        Guid OwnerId) : ModelBase
    {
        public string Manufacturer { get; set; } = Manufacturer;
        public string Type { get; set; } = Type;
        public DateTime RegistrationTime { get; set; } = RegistrationTime;
        public uint NumberOfSeats { get; set; } = NumberOfSeats;
        public string? ImageUrl { get; set; } = ImageUrl;
        public Guid OwnerId { get; set; } = OwnerId;

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<CarEntity, CarDetailModel>()
                    .ReverseMap();
            }
        }

        public static CarDetailModel Empty => new(string.Empty, string.Empty, default, 0, string.Empty, default);
    }
}