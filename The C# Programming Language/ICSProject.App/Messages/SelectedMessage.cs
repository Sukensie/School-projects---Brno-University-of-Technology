using ICSProject.BL.Models;

namespace ICSProject.App.Messages
{
    public record SelectedMessage<T> : Message<T> where T : IModel { }
}
