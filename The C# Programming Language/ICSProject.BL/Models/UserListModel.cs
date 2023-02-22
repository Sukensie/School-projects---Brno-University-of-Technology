using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record UserListModel(string Name) : ModelBase
    {
        public string Name { get; set; } = Name;
        public string? ImageUrl { get; set; }

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<UserEntity, UserListModel>();
            }
        }
    }
}