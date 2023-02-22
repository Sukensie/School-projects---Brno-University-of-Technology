using ICSProject.BL.Models;

namespace ICSProject.App.Messages
{
    public record DeleteMessage<T> : Message<T> where T : IModel { }
}