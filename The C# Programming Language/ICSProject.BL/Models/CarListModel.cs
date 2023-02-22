using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record CarListModel(
        string Type,
        string Manufacturer,
        string? ImageUrl) : ModelBase
    {
        public string Type { get; set; } = Type;
        public string Manufacturer { get; set; } = Manufacturer;
        public string? ImageUrl { get; set; } = ImageUrl;

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<CarEntity, CarListModel>();
            }
        }
    }
}