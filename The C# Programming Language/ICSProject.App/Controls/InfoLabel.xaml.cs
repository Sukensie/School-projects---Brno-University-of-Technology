using System.Windows;
using System.Windows.Input;

namespace ICSProject.App.Controls
{
    public partial class InfoLabel
    {
        public string Title
        {
            get => (string)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value); 
        }

        public string Txt
        {
            get => (string)GetValue(TxtProperty);
            set => SetValue(TxtProperty, value); 
        }


        public static readonly DependencyProperty TxtProperty = DependencyProperty.Register(
            nameof(Txt),
            typeof(string),
            typeof(InfoLabel),
            new PropertyMetadata(default(string)));

        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title),
            typeof(string),
            typeof(InfoLabel),
            new PropertyMetadata(default(string)));

        public InfoLabel()
        {
            InitializeComponent();
        }
    }
}
